
#include "ProjectileWeapon.h"
#include "Projectile.h"
#include "Engine/SkeletalMeshSocket.h"


void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* InstigatorPawn = Cast<APawn>(GetOwner()); //Getting the player that spawned the projectile and convert it to a pawn

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
	UWorld* World = GetWorld();
	if (MuzzleFlashSocket && World)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		//From muzzle flash socket to hit location from TraceUnderCrosshairs
		FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();
			FActorSpawnParameters SpawnParams;// Allow us to set properties for the actor that will be spawned
			SpawnParams.Owner = GetOwner(); //Gets the owner of the weapon.
			SpawnParams.Instigator = InstigatorPawn; // Player responsable for spawning the projectile

			AProjectile* SpawnProjectile = nullptr;

			if (bUseServerSideRewind)
			{
				if (InstigatorPawn->HasAuthority()) //server
				{
					if (InstigatorPawn->IsLocallyControlled()) //server, host - use replicated projectile, no SSR
					{
						SpawnProjectile =World->SpawnActor<AProjectile>(ProjectileClass,SocketTransform.GetLocation(),TargetRotation,SpawnParams);
						SpawnProjectile->bUseServerSideRewind = false;
						SpawnProjectile->Damage = Damage;
						SpawnProjectile->HeadShotDamage = HeadShotDamage;
					}
					else //Server not locally controlled - spawn-replicated projectile, SSR. Player spawn projectile locally already
					{
						// When replicated, clients bullets will be seen by other clients
						SpawnProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
						SpawnProjectile->bUseServerSideRewind = true;
					}
				}
				else // Client - Using SSR
				{
					if (InstigatorPawn->IsLocallyControlled()) // Client, Locally controlled - spawn non-replicated projectile, use SSR
					{
						SpawnProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
						SpawnProjectile->bUseServerSideRewind = true;
						SpawnProjectile->TraceStart = SocketTransform.GetLocation();
						SpawnProjectile->InitialVelocity = SpawnProjectile->GetActorForwardVector() * SpawnProjectile->InitialSpeed;
					}
					else // Client not locally controlled - spawn non-replicated projectile, no SSR
					{
						SpawnProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
						SpawnProjectile->bUseServerSideRewind = false;
					}
				}
			}
			else // Weapon not using SSR. Happens when player has  too much lag
			{
				if (InstigatorPawn->HasAuthority())
				{
					// Authoritative version of the laggy person. Will not happen locally. First on server then on its screen
					SpawnProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					SpawnProjectile->bUseServerSideRewind = false;
					SpawnProjectile->Damage = Damage;
					SpawnProjectile->HeadShotDamage = HeadShotDamage;
				}
			}
	}
}
