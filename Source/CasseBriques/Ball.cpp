// Fill out your copyright notice in the Description page of Project Settings.

#include "Ball.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Paddle.h"
#include "Kismet/GameplayStatics.h"
#include "BreakoutGameMode.h"
#include "Materials/MaterialInterface.h"

// Sets default values
ABall::ABall()
{
    PrimaryActorTick.bCanEverTick = true;

    // Collision principale de la balle
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;

    CollisionSphere->InitSphereRadius(20.0f);
    CollisionSphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));

    // Mesh visuel
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    BallMesh->SetupAttachment(CollisionSphere);
    BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Gestion du déplacement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = CollisionSphere;

    ProjectileMovement->InitialSpeed = 600.0f;
    ProjectileMovement->MaxSpeed = 600.0f;

    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 1.0f;
    ProjectileMovement->Friction = 0.0f;

    // Pas de gravité
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    ProjectileMovement->OnProjectileBounce.AddDynamic(this, &ABall::OnBallBounce);
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
    Super::BeginPlay();

    Paddle = Cast<APaddle>(
        UGameplayStatics::GetActorOfClass(this, APaddle::StaticClass())
    );

    bIsLaunched = false;

    ProjectileMovement->Velocity = FVector::ZeroVector;
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsLaunched && Paddle)
    {
        FVector PaddleLocation = Paddle->GetActorLocation();

        FVector BallLocation = PaddleLocation;

        BallLocation.Y += 80.0f;
        BallLocation.Z = 110.0f;

        SetActorLocation(BallLocation);
    }
}

void ABall::LaunchBall()
{
    ABreakoutGameMode* GameMode =
        Cast<ABreakoutGameMode>(UGameplayStatics::GetGameMode(this));

    if (GameMode && GameMode->bIsGameOver)
    {
        return;
    }

    if (bIsLaunched)
    {
        return;
    }

    bIsLaunched = true;

    FVector InitialDirection =
        FVector(1.0f, 1.0f, 0.0f).GetSafeNormal();

    ProjectileMovement->Velocity =
        InitialDirection * ProjectileMovement->InitialSpeed;
}

void ABall::LaunchBallInDirection(FVector Direction)
{
    ABreakoutGameMode* GameMode =
        Cast<ABreakoutGameMode>(UGameplayStatics::GetGameMode(this));

    if (GameMode && GameMode->bIsGameOver)
    {
        return;
    }

    bIsLaunched = true;

    Direction.Z = 0.0f;
    Direction.Normalize();

    ProjectileMovement->Velocity =
        Direction * ProjectileMovement->InitialSpeed;
}

void ABall::ResetBall()
{
    bIsLaunched = false;

    // Arrête complètement la balle
    ProjectileMovement->StopMovementImmediately();

    if (Paddle)
    {
        FVector BallLocation = Paddle->GetActorLocation();

        BallLocation.Y += 80.0f;
        BallLocation.Z = 110.0f;

        SetActorLocation(BallLocation);
    }
}

void ABall::ActivateExplosiveMode()
{
    bIsExplosive = true;

    // Change l'apparence de la balle
    if (BallMesh && ExplosiveBallMaterial)
    {
        BallMesh->SetMaterial(0, ExplosiveBallMaterial);
    }

    // Si le bonus était déjà actif, on recommence les 10 secondes
    GetWorldTimerManager().ClearTimer(ExplosiveTimerHandle);

    GetWorldTimerManager().SetTimer(
        ExplosiveTimerHandle,
        this,
        &ABall::DeactivateExplosiveMode,
        ExplosiveDuration,
        false
    );
}

void ABall::DeactivateExplosiveMode()
{
    bIsExplosive = false;

    // Remet l'apparence normale
    if (BallMesh && NormalBallMaterial)
    {
        BallMesh->SetMaterial(0, NormalBallMaterial);
    }
}

bool ABall::IsExplosive() const
{
    return bIsExplosive;
}

void ABall::OnBallBounce(
    const FHitResult& ImpactResult,
    const FVector& ImpactVelocity
)
{
    AActor* HitActor = ImpactResult.GetActor();

    if (!HitActor)
    {
        return;
    }

    // Paddle
    if (HitActor->IsA<APaddle>())
    {
        // Le retour sur la raquette casse le combo
        ABreakoutGameMode* GameMode =
            Cast<ABreakoutGameMode>(UGameplayStatics::GetGameMode(this));

        if (GameMode)
        {
            GameMode->ResetCombo();
        }

        if (PaddleHitSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                PaddleHitSound,
                ImpactResult.ImpactPoint
            );
        }

        return;
    }

    // Murs
    const FString HitActorLabel = HitActor->GetActorNameOrLabel();

    if (
        HitActorLabel.StartsWith(TEXT("Wall_Left")) ||
        HitActorLabel.StartsWith(TEXT("Wall_Right")) ||
        HitActorLabel.StartsWith(TEXT("Wall_Top"))
    )
    {
        if (WallHitSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                WallHitSound,
                ImpactResult.ImpactPoint
            );
        }
    }
}

void ABall::SetIsMainBall(bool bMainBall)
{
    bIsMainBall = bMainBall;
}

bool ABall::IsMainBall() const
{
    return bIsMainBall;
}

void ABall::SetIsInPlay(bool bInPlay)
{
    bIsInPlay = bInPlay;
}

bool ABall::IsInPlay() const
{
    return bIsInPlay;
}