// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "BreakoutGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CASSEBRIQUES_API ABreakoutGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	public:
        ABreakoutGameMode();
	
	protected:
		virtual void BeginPlay() override;
    
	public:
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
        int32 Score = 0;
    
        UFUNCTION(BlueprintCallable, Category = "Score")
        void AddScore(int32 Amount);
	
		// ---------- Gestion du combo ----------

		// Multiplicateur actuel : de x1 à x5
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combo")
		int32 ComboMultiplier = 1;

		// Augmente le combo après la destruction d'une brique
		UFUNCTION(BlueprintCallable, Category = "Combo")
		void IncreaseCombo();

		// Remet le combo à x1 lorsque la balle touche la raquette
		UFUNCTION(BlueprintCallable, Category = "Combo")
		void ResetCombo();
	
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
		int32 Lives = 3;

		UFUNCTION(BlueprintCallable, Category = "Game")
		void LoseLife();
	
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
		bool bIsGameOver = false;

		UFUNCTION(BlueprintCallable, Category = "Game")
		void GameOver();
		
		// ---------- Gestion du niveau ----------
        
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
		int32 RemainingBricks = 0;
        
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
		bool bIsLevelComplete = false;
        
		UFUNCTION(BlueprintCallable, Category = "Game")
		void SetBrickCount(int32 Count);
        
		UFUNCTION(BlueprintCallable, Category = "Game")
		void BrickDestroyed();
        
		UFUNCTION(BlueprintCallable, Category = "Game")
		void LevelComplete();
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> LevelCompleteWidgetClass;
			
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> GameOverWidgetClass;
};