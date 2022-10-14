// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileGranade.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AProjectileGranade : public AProjectile
{
	GENERATED_BODY()
public:
	AProjectileGranade();
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

private:
	UPROPERTY(EditAnywhere)
	USoundCue* BounceSound;
	virtual void Destroyed() override;
};
