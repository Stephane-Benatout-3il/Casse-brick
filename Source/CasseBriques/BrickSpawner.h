// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrickSpawner.generated.h"

UCLASS()
class CASSEBRIQUES_API ABrickSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrickSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bricks")
	TSubclassOf<class ABrick> BrickClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bricks")
	int32 Rows = 4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bricks")
	int32 Columns = 8;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bricks")
	float HorizontalSpacing = 170.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bricks")
	float VerticalSpacing = 80.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bricks|Random")
	float BrickSpawnChance = 0.65f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bricks|Random")
	int32 MinBrickHealth = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bricks|Random")
	int32 MaxBrickHealth = 5;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};