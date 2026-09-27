// Fill out your copyright notice in the Description page of Project Settings.

#include "BrickSpawner.h"
#include "Brick.h"
#include "Engine/World.h"
#include "BreakoutGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABrickSpawner::ABrickSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABrickSpawner::BeginPlay()
{
	Super::BeginPlay();
    
	if (!BrickClass)
	{
		return;
	}

	const FVector StartLocation = GetActorLocation();

	int32 SpawnedBrickCount = 0;

	const int32 HalfColumns = Columns / 2;

	for (int32 Row = 0; Row < Rows; Row++)
	{
		for (int32 Column = 0; Column < HalfColumns; Column++)
		{
			// Décide aléatoirement si cette paire de briques existe
			if (FMath::FRand() > BrickSpawnChance)
			{
				continue;
			}

			// Même nombre de PV pour les deux briques symétriques
			const int32 RandomHealth =
				FMath::RandRange(MinBrickHealth, MaxBrickHealth);

			// ---------- Brique de gauche ----------

			FVector LeftLocation = StartLocation;

			LeftLocation.X += Column * HorizontalSpacing;
			LeftLocation.Y += Row * VerticalSpacing;

			ABrick* LeftBrick = GetWorld()->SpawnActor<ABrick>(
				BrickClass,
				LeftLocation,
				FRotator::ZeroRotator
			);

			if (LeftBrick)
			{
				LeftBrick->SetHealth(RandomHealth);
				SpawnedBrickCount++;
			}

			// ---------- Brique symétrique ----------

			const int32 MirrorColumn = Columns - 1 - Column;

			FVector RightLocation = StartLocation;

			RightLocation.X += MirrorColumn * HorizontalSpacing;
			RightLocation.Y += Row * VerticalSpacing;

			ABrick* RightBrick = GetWorld()->SpawnActor<ABrick>(
				BrickClass,
				RightLocation,
				FRotator::ZeroRotator
			);

			if (RightBrick)
			{
				RightBrick->SetHealth(RandomHealth);
				SpawnedBrickCount++;
			}
		}
	}

	ABreakoutGameMode* GameMode =
        Cast<ABreakoutGameMode>(UGameplayStatics::GetGameMode(this));
    
    if (GameMode)
    {
        GameMode->SetBrickCount(SpawnedBrickCount);
    }
}

// Called every frame
void ABrickSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}