// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

class APaddle;
class USoundBase;
class UMaterialInterface;

UCLASS()
class CASSEBRIQUES_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

	UFUNCTION(BlueprintCallable, Category = "Ball")
	void ResetBall();

	UFUNCTION(BlueprintCallable, Category = "Ball")
	void LaunchBall();

	UFUNCTION(BlueprintCallable, Category = "Ball")
	void LaunchBallInDirection(FVector Direction);
	
	UFUNCTION(BlueprintCallable, Category = "Ball")
	void SetIsMainBall(bool bMainBall);

	UFUNCTION(BlueprintCallable, Category = "Ball")
	bool IsMainBall() const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball")
	bool bIsInPlay = true;
	
	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	void ActivateExplosiveMode();

	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	bool IsExplosive() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
	APaddle* Paddle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball")
	bool bIsLaunched = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball")
	bool bIsMainBall = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* BallMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    USoundBase* PaddleHitSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* WallHitSound;
	
	// ----- Power-Up : balle explosive -----

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PowerUp")
	bool bIsExplosive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	float ExplosiveDuration = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp|Visual")
	UMaterialInterface* NormalBallMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp|Visual")
	UMaterialInterface* ExplosiveBallMaterial;

	FTimerHandle ExplosiveTimerHandle;

	void DeactivateExplosiveMode();
	
	UFUNCTION()
	void OnBallBounce(
		const FHitResult& ImpactResult,
		const FVector& ImpactVelocity
	);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Ball")
	void SetIsInPlay(bool bInPlay);

	UFUNCTION(BlueprintCallable, Category = "Ball")
	bool IsInPlay() const;

};