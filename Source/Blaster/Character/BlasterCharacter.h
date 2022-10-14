// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "Components/TimelineComponent.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "Blaster/BlasterTypes/Team.h"
#include "BlasterCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//Function that register replicated variables
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	// Play Montages
	void PlayFireMontage(bool bAiming);
	void PlayReloadMontage();
	void PlaySwapMontage();
	void PlayElimMontage();
	void PlayThrowGranadeMontage();

	virtual void OnRep_ReplicatedMovement() override;
	virtual void Destroyed() override;
	void Elim(bool bPlayerLeftGame);
	UFUNCTION(NetMulticast, Reliable)
		void MulticastElim(bool bPlayerLeftGame);
	UFUNCTION(BlueprintImplementableEvent)
		void ShowSniperScopeWidget(bool bShowScoper);
	UPROPERTY(Replicated)
		bool bDisableGameplay = false;

	void UpdateHUDHealth();
	void UpdateHUDShield();
	void UpdateHUDGranades();
	void UpdateHUDAmmo();
	void SpawnDefaultWeapon();

	UPROPERTY()
		TMap<FName, class UBoxComponent*> HitCollisionBoxes;

	bool bFinishedSwapping = false;

	UFUNCTION(Server, Reliable)
	void ServerLeaveGame();

	FOnLeftGame OnLeftGame;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastGainedTheLead();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLostTheLead();

	void SetTeamColor(ETeam Team);

protected:
	virtual void BeginPlay() override;
	virtual void Jump() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void ReloadButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	//The purpose of this function is to get a yaw value for the AimOffsets in the animation BP
	void AimOffset(float DeltaTime);
	void CalculateAO_Pitch();
	void SimProxiesTurn();
	void FireButtonPressed();
	void FireButtonReleased();
	void PlayHitReactMontage();
	void GranadeButtonPressed();
	void DropOrDestroyWeapon(AWeapon* Weapon);
	void DropOrDestroyWeapons();
	void SetSpawnPoint();
	void OnPlayerStateInitialized();

	UFUNCTION()
		void ReceiveDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	UFUNCTION()
		// Poll for any relevant classes and intialize the HUD
		void PollInit();

	void RotateInPlace(float DeltaTime);

	// Hit boxes used for server side rewind

	UPROPERTY(EditAnywhere)
		class UBoxComponent* head;

	UPROPERTY(EditAnywhere)
		UBoxComponent* pelvis;

	UPROPERTY(EditAnywhere)
		UBoxComponent* spine_02;

	UPROPERTY(EditAnywhere)
		UBoxComponent* spine_03;

	UPROPERTY(EditAnywhere)
		UBoxComponent* upperarm_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* upperarm_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* lowerarm_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* lowerarm_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* hand_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* hand_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* backpack;

	UPROPERTY(EditAnywhere)
		UBoxComponent* blanket;

	UPROPERTY(EditAnywhere)
		UBoxComponent* thigh_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* thigh_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* calf_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* calf_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* foot_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* foot_r;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* OverheadWidget;

	// Replicated so that the value set in the server can be replicated to the clients
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
		class AWeapon* OverlappingWeapon;

	UFUNCTION()
		void OnRep_OverlappingWeapon(AWeapon* LastWeapon);



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere)
		class UBuffComponent* Buff;

	// Blaster Components
	UPROPERTY(VisibleAnywhere)
		class ULagCompensationComponent* LagCompensation;


	UFUNCTION(Server, Reliable)
		void ServerEquipButtonPressed();


	// Calculating Yaw and Pitch Aim Offsets used in the BlasterAnimInstance class
	float AO_Yaw;
	float AO_Pitch;

	float InterpAO_Yaw;
	FRotator StartingAimRotation;

	// Enum set in the "Blaster/BlasterTypes/TurningInPlace.h" file
	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	// Animation montages
	UPROPERTY(EditAnywhere, Category = Combat)
		class UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* SwapMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* ElimMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* ThrowGranadeMontage;

	void HideCharacterIfCameraClose();

	UPROPERTY(EditAnywhere)
		float CameraThreshold = 200.f;

	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();


	// Player health
	UPROPERTY(EditAnywhere, Category = "Player Stats")
		float MaxHealth = 100.f;
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Playeer Stats")
		float Health = 100.f;
	UFUNCTION()
		void OnRep_Health(float LastHealth);

	//Player Shield
	UPROPERTY(EditAnywhere, Category = "Player Stats")
		float MaxShield = 100.f;
	UPROPERTY(ReplicatedUsing = OnRep_Shield, EditAnywhere, Category = "Playeer Stats")
		float Shield = 100.f;
	UFUNCTION()
		void OnRep_Shield(float LastShield);

	UPROPERTY()
		class ABlasterPlayerController* BlasterPlayerController;
	bool bElimmed = false;
	FTimerHandle ElimTimer;
	UPROPERTY(EditDefaultsOnly)
		float ElimDelay = 3.f;
	void ElimTimerFinished();
	bool bLeftGame = false;

	//Dissolve effect
	UPROPERTY(VisibleAnywhere)
		UTimelineComponent* DissolveTimeline;
	//Track that we use on the time line. Where we add keyframes 
	FOnTimelineFloat DissolveTrack;

	/*Callback function that is called every frame that the time line is updated
	It recieves the float value that tells where we are on the curve*/
	UFUNCTION()
		void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();
	UPROPERTY(EditAnywhere)
		UCurveFloat* DissolveCurve;

	float cuatre;

	// Stores the dynamic material we create in run time
	UPROPERTY(VisibleAnywhere, Category = "Elim")
		UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	// Actual material instance that we created in BP and what the Dynamic material uses
	UPROPERTY(VisibleAnywhere, Category = "Elim")
		UMaterialInstance* DissolveMaterialInstance;

	//Team Colors
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* RedDissolveMatInst;

	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* BlueDissolveMatInst;

	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* RedMaterial;

	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* BlueMaterial;

	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* OriginalMaterial;

	//Elim effects
	UPROPERTY(EditAnywhere)
		UParticleSystem* ElimBotEffect; // set in BP

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* ElimBotComponent; //Set once the effect is spwaned

	UPROPERTY(EditAnywhere)
		class USoundCue* ElimBotSound;

	UPROPERTY()
		class ABlasterPlayerState* BlasterPlayerState;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* CrownSystem;

	UPROPERTY()
	class UNiagaraComponent* CrownComponent;

	//Granade
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* AttachedGranade;

	//Default weapon
	UPROPERTY(EditAnywhere)
		TSubclassOf<AWeapon> DefaultWeaponClass;

	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;

public:
	//Used on the weapon class on the OnSphereOverlap
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	AWeapon* GetEquippedWeapon();
	AWeapon* GetSecondaryWeapon();
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FVector GetHitTarget() const;
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetShield() const { return Shield; }
	FORCEINLINE void SetShield(float Amount) { Shield = Amount; }
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetMaxShield() const { return MaxShield; }
	ECombatState GetCombatState() const;
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE UBuffComponent* GetBuff() const { return Buff; }
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }
	FORCEINLINE UStaticMeshComponent* GetAttachedGranade() const { return AttachedGranade; }
	bool IsLocallyReloading();
	FORCEINLINE ULagCompensationComponent* GetLagCoC() const { return LagCompensation; }
	FORCEINLINE bool IsHoldingTheFlag() const;
	ETeam GetTeam();
	void SetHoldingTheFlag(bool bHolding);
};
