// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/BlasterComponents/LagCompensationComponent.h"
#include "Kismet/GameplayStatics.h"

 AProjectileBullet::AProjectileBullet()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true; // bullet will keep the rotation aliened with velocity
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = InitialSpeed;
}
#if WITH_EDITOR
 void AProjectileBullet::PostEditChangeProperty(FPropertyChangedEvent& Event)
 {
	 Super::PostEditChangeProperty(Event);

	 FName PropertyName = Event.Property != nullptr ? Event.Property->GetFName() : NAME_None;

	 if (PropertyName == GET_MEMBER_NAME_CHECKED(AProjectileBullet, InitialSpeed))
	 {
		 if (ProjectileMovementComponent)
		 {
			 ProjectileMovementComponent->InitialSpeed = InitialSpeed;
			 ProjectileMovementComponent->MaxSpeed = InitialSpeed;
		 }
	 }
 }
#endif
 void AProjectileBullet:: OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ABlasterCharacter* OwnerCharacter = Cast<ABlasterCharacter>(GetOwner());
	if (OwnerCharacter)
	{
		ABlasterPlayerController* OwnerController = Cast<ABlasterPlayerController>(OwnerCharacter->Controller);
		if (OwnerController)
		{
			if (OwnerCharacter->HasAuthority() && !bUseServerSideRewind) //UseServerSideRewind is being decided in ProjectileWeapon
			{
				const float DamageToCause = Hit.BoneName.ToString() == FString("head") ? HeadShotDamage : Damage;
				UGameplayStatics::ApplyDamage(OtherActor, DamageToCause, OwnerController, this, UDamageType::StaticClass());
				Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
				return;
			}
			ABlasterCharacter* HitCharacter = Cast<ABlasterCharacter>(OtherActor);
			if (bUseServerSideRewind && OwnerCharacter->GetLagCoC() && OwnerCharacter->IsLocallyControlled() && HitCharacter)
			{
				OwnerCharacter->GetLagCoC()->ProjectileServerScoreRequest(
					HitCharacter,
					TraceStart,
					InitialVelocity,
					OwnerController->GetServerTime() - OwnerController->SingleTripTime
				);
			}
		}
	}

	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit); // Just destroys the projectile
}

void AProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
	/*
	FPredictProjectilePathParams PathParams;
	PathParams.bTraceWithChannel = true; //Should be tracing into a particular channel
	PathParams.bTraceWithCollision = true; //Allow us to predict projectile paths and do hit events
	PathParams.DrawDebugTime = 5.f;
	PathParams.DrawDebugType = EDrawDebugTrace::ForDuration; //Way that we want to draw the debugg
	PathParams.LaunchVelocity = GetActorForwardVector() * InitialSpeed;//Determines the path of the projectile
	PathParams.MaxSimTime = 4.f; //Amount of time projectile is flying
	PathParams.ProjectileRadius = 5.f;//The sphere radius of the sphere traces in the projectile path
	PathParams.SimFrequency = 30.f;//How much traces it should do on the parabola shape
	PathParams.StartLocation = GetActorLocation();
	PathParams.TraceChannel = ECollisionChannel::ECC_Visibility; //Channel that we are tracing to
	PathParams.ActorsToIgnore.Add(this); //Ignore the bullet itself

	FPredictProjectilePathResult PathResult;

	UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);*/
}
