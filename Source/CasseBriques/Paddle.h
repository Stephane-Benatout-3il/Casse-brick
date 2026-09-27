// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Paddle.generated.h"

UCLASS()
class CASSEBRIQUES_API APaddle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APaddle();

protected:
	// Called when the game starts or when spawned
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* PaddleMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* LaunchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* PauseAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> PauseMenuWidgetClass;
	
	UPROPERTY()
	class UUserWidget* PauseMenuWidget;
	
	// ----- Power-Up : agrandissement de la raquette -----

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	float PaddleSizeMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	float PaddleSizeDuration = 10.0f;
	
	// ----- Power-Up : Multiball -----

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PowerUp")
	TSubclassOf<class ABall> BallClass;

	FVector OriginalPaddleScale;

	FTimerHandle PaddleSizeTimerHandle;

	void ResetPaddleSize();
	
	
	
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void MoveHorizontal(const struct FInputActionValue& Value);
	
	void LaunchBall();
	
	void PauseGame();
	
	UFUNCTION(BlueprintCallable, Category = "Game")
	void ResumeGame();
	
	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	void ActivatePaddleSizePowerUp();
	
	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	void ActivateMultiballPowerUp();
};