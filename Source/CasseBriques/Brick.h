// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Brick.generated.h"

class UNiagaraSystem;
class UMaterialInterface;
class USoundBase;
class APowerUp;

UCLASS()
class CASSEBRIQUES_API ABrick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrick();
	
	UFUNCTION(BlueprintCallable, Category = "Brick")
	void SetHealth(int32 NewHealth);
	
	UFUNCTION(BlueprintCallable, Category = "Brick")
	void TakeExplosionDamage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* BrickMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UNiagaraSystem* BrickExplosionEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* HitSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* DestroySound;
	
	// Classe du Power-Up pouvant apparaître à la destruction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PowerUp")
	TSubclassOf<APowerUp> PowerUpClass;

	// Probabilité d'apparition du Power-Up : 0.20 = 20 %
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp",
		meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float PowerUpSpawnChance = 0.20f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    FLinearColor BrickColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brick")
	int32 MaxHealth = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Brick")
	int32 CurrentHealth = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Brick|Materials")
	UMaterialInterface* Material5HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Brick|Materials")
	UMaterialInterface* Material4HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Brick|Materials")
	UMaterialInterface* Material3HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Brick|Materials")
	UMaterialInterface* Material2HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Brick|Materials")
	UMaterialInterface* Material1HP;

	void UpdateBrickColor();

	UFUNCTION()
	void OnBrickHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
		);
	
	// ----- Power-Up : explosion -----

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp|Explosive")
	float ExplosionRadius = 250.0f;

	void ExplodeNearbyBricks();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};