// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

UENUM(BlueprintType)
enum class EPowerUpType : uint8
{
	PaddleSize    UMETA(DisplayName = "Paddle Size"),
	Multiball     UMETA(DisplayName = "Multiball"),
	ExplosiveBall UMETA(DisplayName = "Explosive Ball")
};

UCLASS()
class CASSEBRIQUES_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Mesh visible du Power-Up
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PowerUpMesh;
	
	// Type de Power-Up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	EPowerUpType PowerUpType = EPowerUpType::PaddleSize;

	// Vitesse de chute
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	float FallSpeed = 300.0f;
	
	UFUNCTION()
	void OnPowerUpOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};