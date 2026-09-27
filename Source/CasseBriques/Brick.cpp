// Fill out your copyright notice in the Description page of Project Settings.

#include "Brick.h"
#include "Components/StaticMeshComponent.h"
#include "BreakoutGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Materials/MaterialInterface.h"
#include "Sound/SoundBase.h"
#include "PowerUp.h"
#include "Ball.h"

// Sets default values
ABrick::ABrick()
{
    // Set this actor to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    BrickMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BrickMesh"));
    RootComponent = BrickMesh;

    BrickMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

    BrickMesh->SetNotifyRigidBodyCollision(true);

    BrickMesh->OnComponentHit.AddDynamic(
        this,
        &ABrick::OnBrickHit
    );
}

// Called when the game starts or when spawned
void ABrick::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;
    UpdateBrickColor();
}

void ABrick::UpdateBrickColor()
{
    UMaterialInterface* NewMaterial = nullptr;

    switch (CurrentHealth)
    {
    case 5:
        NewMaterial = Material5HP;
        break;

    case 4:
        NewMaterial = Material4HP;
        break;

    case 3:
        NewMaterial = Material3HP;
        break;

    case 2:
        NewMaterial = Material2HP;
        break;

    case 1:
        NewMaterial = Material1HP;
        break;

    default:
        return;
    }

    if (BrickMesh && NewMaterial)
    {
        BrickMesh->SetMaterial(0, NewMaterial);
    }
}

void ABrick::OnBrickHit(
    UPrimitiveComponent* HitComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit
)
{
    // Vérifie si l'acteur qui frappe la brique est une balle explosive
    ABall* Ball = Cast<ABall>(OtherActor);

    if (Ball && Ball->IsExplosive())
    {
        ExplodeNearbyBricks();
    }

    // La brique perd 1 PV
    CurrentHealth--;

    // Met à jour sa couleur en fonction des PV restants
    UpdateBrickColor();

    // Récupération du GameMode
    ABreakoutGameMode* GameMode =
        Cast<ABreakoutGameMode>(UGameplayStatics::GetGameMode(this));

    // ---------- La brique a encore des PV ----------
    if (CurrentHealth > 0)
    {
        // 50 points pour un impact
        if (GameMode)
        {
            GameMode->AddScore(50);
            GameMode->IncreaseCombo();
        }

        // Son d'impact
        if (HitSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                HitSound,
                GetActorLocation()
            );
        }

        return;
    }

    // ---------- La brique est détruite ----------

    // 100 points + retrait du compteur de briques
    if (GameMode)
    {
        GameMode->AddScore(100);
        GameMode->IncreaseCombo();
        GameMode->BrickDestroyed();
    }

    // Son de destruction
    if (DestroySound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            DestroySound,
            GetActorLocation()
        );
    }

    // Effet de particules
    if (BrickExplosionEffect)
    {
        UNiagaraComponent* NiagaraComponent =
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                BrickExplosionEffect,
                GetActorLocation(),
                GetActorRotation()
            );

        if (NiagaraComponent)
        {
            NiagaraComponent->SetVariableLinearColor(
                FName("User.BrickColor"),
                BrickColor
            );
        }
    }

    // ---------- Apparition éventuelle d'un Power-Up ----------
    if (PowerUpClass && FMath::FRand() <= PowerUpSpawnChance)
    {
        GetWorld()->SpawnActor<APowerUp>(
            PowerUpClass,
            GetActorLocation(),
            FRotator::ZeroRotator
        );
    }

    // Destruction de l'acteur
    Destroy();
}

// Called every frame
void ABrick::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABrick::SetHealth(int32 NewHealth)
{
    MaxHealth = FMath::Clamp(NewHealth, 1, 5);
    CurrentHealth = MaxHealth;

    UpdateBrickColor();
}

void ABrick::TakeExplosionDamage()
{
    // Évite de retraiter une brique déjà détruite
    if (CurrentHealth <= 0)
    {
        return;
    }

    CurrentHealth--;

    // La brique survit : changement de couleur
    if (CurrentHealth > 0)
    {
        UpdateBrickColor();

        // Même récompense qu'un impact normal
        ABreakoutGameMode* GameMode =
            Cast<ABreakoutGameMode>(
                UGameplayStatics::GetGameMode(this)
            );

        if (GameMode)
        {
            GameMode->AddScore(50);
        }

        return;
    }

    // ---------- La brique est détruite par l'explosion ----------

    ABreakoutGameMode* GameMode =
        Cast<ABreakoutGameMode>(
            UGameplayStatics::GetGameMode(this)
        );

    if (GameMode)
    {
        GameMode->AddScore(100);
        GameMode->BrickDestroyed();
    }

    // Son de destruction
    if (DestroySound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            DestroySound,
            GetActorLocation()
        );
    }

    // Particules de destruction
    if (BrickExplosionEffect)
    {
        UNiagaraComponent* NiagaraComponent =
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                BrickExplosionEffect,
                GetActorLocation(),
                GetActorRotation()
            );

        if (NiagaraComponent)
        {
            NiagaraComponent->SetVariableLinearColor(
                FName("User.BrickColor"),
                BrickColor
            );
        }
    }

    Destroy();
}

void ABrick::ExplodeNearbyBricks()
{
    TArray<AActor*> Bricks;

    UGameplayStatics::GetAllActorsOfClass(
        this,
        ABrick::StaticClass(),
        Bricks
    );

    const FVector ExplosionLocation = GetActorLocation();

    for (AActor* Actor : Bricks)
    {
        ABrick* NearbyBrick = Cast<ABrick>(Actor);

        if (!NearbyBrick || NearbyBrick == this)
        {
            continue;
        }

        const float Distance = FVector::Dist(
            ExplosionLocation,
            NearbyBrick->GetActorLocation()
        );

        if (Distance <= ExplosionRadius)
        {
            NearbyBrick->TakeExplosionDamage();
        }
    }
}