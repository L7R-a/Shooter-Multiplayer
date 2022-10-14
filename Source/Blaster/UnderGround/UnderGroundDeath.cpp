// Diego La Rosa 2022

#include "UnderGroundDeath.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Weapon/Weapon.h"
#include "Components/BoxComponent.h"

AUnderGroundDeath::AUnderGroundDeath()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(RootComponent);
	OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	OverlapBox->AddLocalOffset(FVector(0.f, 0.f, 85.f));
}

// Called when the game starts or when spawned
void AUnderGroundDeath::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AUnderGroundDeath::OnBoxOverlap);
	}
}

void AUnderGroundDeath::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABlasterCharacter* Blaster = Cast<ABlasterCharacter>(OtherActor);
	if (Blaster) 
	{
		if (Blaster->GetEquippedWeapon())
		{
			Blaster->GetEquippedWeapon()->SetActorLocation(
				Blaster->GetEquippedWeapon()->GetSpawnPoint(),
				false,
				OutSweepHitResult,
				ETeleportType::TeleportPhysics
			);
		}
		if (Blaster->GetSecondaryWeapon())
		{
			Blaster->GetSecondaryWeapon()->SetActorLocation(
				Blaster->GetSecondaryWeapon()->GetSpawnPoint(),
				false,
				OutSweepHitResult,
				ETeleportType::TeleportPhysics
			);
		}
		Blaster->Elim(false);
		
	}
}

void AUnderGroundDeath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
