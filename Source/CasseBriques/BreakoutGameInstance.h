// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BreakoutGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CASSEBRIQUES_API UBreakoutGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	// Score conservé entre les niveaux / redémarrages du niveau
	UPROPERTY(BlueprintReadWrite, Category = "Game")
	int32 SavedScore = 0;

	// Vies conservées entre les niveaux / redémarrages du niveau
	UPROPERTY(BlueprintReadWrite, Category = "Game")
	int32 SavedLives = 3;
	
	UFUNCTION(BlueprintCallable, Category = "Game")
	void ResetGame();
};