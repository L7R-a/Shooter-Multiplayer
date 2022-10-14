// Diego La Rosa 2022

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Flag.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AFlag : public AWeapon
{
	GENERATED_BODY()
public:
	AFlag();
	virtual void Dropped() override;
	void ResetFlag();

protected:
	virtual void OnEquip() override;
	virtual void OnDrop() override;
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FlagMesh;
	FTransform InitialTransform;

public:
	FORCEINLINE FTransform GetInitialLocation() const { return InitialTransform; }
};
