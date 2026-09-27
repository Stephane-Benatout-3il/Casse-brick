// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathZone.h"
#include "Components/BoxComponent.h"
#include "Ball.h"
#include "BreakoutGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADeathZone::ADeathZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionBox->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void ADeathZone::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(
		this,
		&ADeathZone::OnOverlapBegin
	);
}

void ADeathZone::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    ABall* LostBall = Cast<ABall>(OtherActor);

    if (!LostBall)
    {
        return;
    }

    // Cette balle n'est désormais plus considérée comme étant en jeu.
    LostBall->SetIsInPlay(false);

    // Recherche toutes les balles présentes.
    TArray<AActor*> Balls;

    UGameplayStatics::GetAllActorsOfClass(
        this,
        ABall::StaticClass(),
        Balls
    );

    ABall* MainBall = nullptr;
    bool bAnotherBallIsInPlay = false;

    for (AActor* Actor : Balls)
    {
        ABall* Ball = Cast<ABall>(Actor);

        if (!Ball)
        {
            continue;
        }

        // On garde une référence vers la balle principale.
        if (Ball->IsMainBall())
        {
            MainBall = Ball;
        }

        // Vérifie s'il reste une autre balle active.
        if (
            Ball != LostBall &&
            Ball->IsInPlay()
        )
        {
            bAnotherBallIsInPlay = true;
        }
    }

    // ---------------------------------------------------------
    // IL RESTE AU MOINS UNE AUTRE BALLE
    // ---------------------------------------------------------

    if (bAnotherBallIsInPlay)
    {
        if (LostBall->IsMainBall())
        {
            // La balle principale doit être conservée pour
            // pouvoir la réutiliser à la prochaine vie.

            LostBall->SetActorHiddenInGame(true);
            LostBall->SetActorEnableCollision(false);
        }
        else
        {
            // Une balle secondaire peut simplement être détruite.
            LostBall->Destroy();
        }
        return;
    }

    // ---------------------------------------------------------
    // DERNIERE BALLE PERDUE
    // ---------------------------------------------------------
    // Si la dernière balle était secondaire, on la détruit.
    if (!LostBall->IsMainBall())
    {
        LostBall->Destroy();
    }
    else
    {
        MainBall = LostBall;
    }

    // Récupération du GameMode.
    ABreakoutGameMode* GameMode =
        Cast<ABreakoutGameMode>(
            UGameplayStatics::GetGameMode(this)
        );

    // Son de perte de vie.
    if (LoseLifeSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            LoseLifeSound,
            LostBall->GetActorLocation()
        );
    }

    // Retire une seule vie.
    if (GameMode)
    {
        GameMode->LoseLife();
    }

    // ---------------------------------------------------------
    // IL RESTE DES VIES
    // ---------------------------------------------------------

    if (
        GameMode &&
        !GameMode->bIsGameOver &&
        MainBall
    )
    {
        MainBall->SetActorHiddenInGame(false);
        MainBall->SetActorEnableCollision(true);

        MainBall->SetIsInPlay(true);
        MainBall->ResetBall();
    }

    // ---------------------------------------------------------
    // GAME OVER
    // ---------------------------------------------------------

    else if (MainBall)
    {
        MainBall->SetActorHiddenInGame(true);
        MainBall->SetActorEnableCollision(false);
    }
}