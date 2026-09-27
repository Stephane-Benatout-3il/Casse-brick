// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerUp.h"
#include "Components/StaticMeshComponent.h"
#include "Paddle.h"
#include "Ball.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PowerUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("PowerUpMesh")
	);

	RootComponent = PowerUpMesh;

	// Le Power-Up ne doit pas utiliser la physique
	PowerUpMesh->SetSimulatePhysics(false);

	// Pour l'instant, il ne bloque rien
	PowerUpMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	PowerUpMesh->SetGenerateOverlapEvents(true);

	PowerUpMesh->OnComponentBeginOverlap.AddDynamic(
		this,
		&APowerUp::OnPowerUpOverlap
	);

}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();

	// Choix aléatoire entre les 3 Power-Ups
	const int32 RandomType = FMath::RandRange(0, 2);

	switch (RandomType)
	{
	case 0:
		PowerUpType = EPowerUpType::PaddleSize;
		break;

	case 1:
		PowerUpType = EPowerUpType::Multiball;
		break;

	case 2:
		PowerUpType = EPowerUpType::ExplosiveBall;
		break;
	}
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector NewLocation = GetActorLocation();

	// Dans ton terrain, la descente se fait sur l'axe Y
	NewLocation.Y -= FallSpeed * DeltaTime;

	SetActorLocation(NewLocation);
}

void APowerUp::OnPowerUpOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	APaddle* Paddle = Cast<APaddle>(OtherActor);

	if (!Paddle)
	{
		return;
	}

	switch (PowerUpType)
	{
	case EPowerUpType::PaddleSize:
		{
			Paddle->ActivatePaddleSizePowerUp();
			break;
		}

	case EPowerUpType::Multiball:
		{
			Paddle->ActivateMultiballPowerUp();
			break;
		}
	case EPowerUpType::ExplosiveBall:
		{
			TArray<AActor*> Balls;

			UGameplayStatics::GetAllActorsOfClass(
				this,
				ABall::StaticClass(),
				Balls
			);

			for (AActor* Actor : Balls)
			{
				ABall* Ball = Cast<ABall>(Actor);

				if (Ball)
				{
					Ball->ActivateExplosiveMode();
				}
			}
			break;
		}
	}

	Destroy();
}