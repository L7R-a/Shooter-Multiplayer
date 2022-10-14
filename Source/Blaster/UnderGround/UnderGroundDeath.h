// Diego La Rosa 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnderGroundDeath.generated.h"

UCLASS()
class BLASTER_API AUnderGroundDeath : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnderGroundDeath();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnBoxOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);
private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* OverlapBox;

	FHitResult* OutSweepHitResult;

public:	
	virtual void Tick(float DeltaTime) override;

};
