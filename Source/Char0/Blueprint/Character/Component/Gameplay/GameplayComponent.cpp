#include "GameplayComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Char0/Blueprint/Character/LttnCharacter.h"
#include "Char0/Blueprint/Character/Bot/BotCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

UGameplayComponent::UGameplayComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UGameplayComponent::BeginPlay()
{
	Super::BeginPlay();

	// UKismetSystemLibrary::PrintString(GetWorld(), "UGameplayComponent::BeginPlay \n\t " + PlayerManager.Player.ToString() + " \n\t " + PlayerManager.Weapon.ToString(), true, true, FLinearColor::Red,
	// 25.0f);
}

void UGameplayComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGameplayComponent, PlayerManager);
}

FPlayerManager UGameplayComponent::GetPlayerManager() const
{
	return PlayerManager;
}

void UGameplayComponent::SetNewPlayerInfo()
{
	PlayerManager = FPlayerManager( //Set after spawned
		StartingScore, //todo ?
		Health,
		MaxHealth,
		HealthCooldown,
		HealthRegenRate,
		ReloadTime,
		Projectiles,
		MaxProjectiles,
		Stamina,
		MaxStamina,
		StaminaRegenRate,
		ProjectileCount,
		MaxWeaponProjectiles,
		WeaponDamage,
		FireRate,
		Penetration,
		Range
	);
}

void UGameplayComponent::LevelStarted()
{
	// BotDamage++;
}

void UGameplayComponent::SetMaxHealth()
{
	PlayerManager.SetPlayerHealthMax();
}

void UGameplayComponent::TakeDamage(const float Damage, const FVector& DamageDirection)
{
	constexpr float Knockback = 500;

	//todo player hit instead, or as well?
	Cast<ACharacter>(GetOwner())->LaunchCharacter(FVector(DamageDirection.X, DamageDirection.Y, 0.0f) * Knockback, true, false);

	if (const float NewHealth = PlayerManager.UpdatePlayerHealth(-1 * Damage); NewHealth <= 0)
	{
		GetCharacter()->PlayerDead();
	}
	else
	{
		GetCharacter()->SetPlayerHealth(NewHealth);
		GetWorld()->GetTimerManager().ClearTimer(HealthCooldownTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(HealthCooldownTimerHandle, this, &UGameplayComponent::BeginHealthRegen,
		                                       PlayerManager.Player.HealthCooldown, false);
	}
}

void UGameplayComponent::StartFiring()
{
	//if !isAiming then aim first then fire

	if (bIsReloading or bFireRateCooldownActive /*or bIsAiming*/)
	{
		//TODO cant if sprinting, need to be aiming too
		return;
	}
	if (PlayerManager.CanFire())
	{
		bFireRateCooldownActive = true;
		bShouldStopFiring = false;
		TryFire(); //TODO do we need this?
		GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &UGameplayComponent::StartedFiring,
		                                       PlayerManager.Weapon.FireRate, true);
	}
}

void UGameplayComponent::StopFiring()
{
	bShouldStopFiring = true;
}

void UGameplayComponent::Reload()
{
	//TODO Reload cancel, can't sprint while reloading
	if (PlayerManager.CanReload())
	{
		bIsReloading = true;
		GetCharacter()->PlayerIsReloading(PlayerManager.Player.ReloadTime);

		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &UGameplayComponent::ReloadComplete,
		                                       PlayerManager.Player.ReloadTime, false);
	}
}

void UGameplayComponent::SetIsMoving(const bool bMoving)
{
	bIsMoving = bMoving;
}

void UGameplayComponent::StartSprinting()
{
	bIsSprinting = true;
	// InputState.bWantsToSprint = true;
	// Server_UpdateInputState(InputState);
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(StaminaDegenTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(StaminaDegenTimerHandle, this, &UGameplayComponent::DecrementStamina, 0.1f, true);
	StaminaDepletedDelegate.AddDynamic(this, &UGameplayComponent::StopSprinting);
}

void UGameplayComponent::StopSprinting()
{
	bIsSprinting = false;
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(StaminaDegenTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &UGameplayComponent::IncrementStamina, 0.1f, true);
	StaminaDepletedDelegate.RemoveDynamic(this, &UGameplayComponent::StopSprinting);
}

bool UGameplayComponent::CanUpgradeHealth() const
{
	return PlayerManager.CanUpgradeHealth();
}

bool UGameplayComponent::CanUpgradeStamina() const
{
	return PlayerManager.CanUpgradeStamina();
}

bool UGameplayComponent::CanUpgradeReloadSpeed() const
{
	return PlayerManager.CanUpgradeReloadSpeed();
}

bool UGameplayComponent::CanUpgradeWeapon() const
{
	return PlayerManager.CanUpgradeWeapon();
}

bool UGameplayComponent::CanUpgradeProjectileCapacity() const
{
	return PlayerManager.CanUpgradeProjectileCapacity();
}

void UGameplayComponent::UpgradeHealth()
{
	if (PlayerManager.CanUpgradeHealth())
	{
		GetCharacter()->ShowScore(PlayerManager.CurrentScore, PlayerManager.UpgradeHealth());
		BeginHealthRegen();
	}
}

void UGameplayComponent::UpgradeStamina()
{
	if (PlayerManager.CanUpgradeStamina())
	{
		GetCharacter()->ShowScore(PlayerManager.CurrentScore, PlayerManager.UpgradeStamina());
		StopSprinting();
	}
}

void UGameplayComponent::UpgradeReloadSpeed()
{
	if (PlayerManager.CanUpgradeReloadSpeed())
	{
		GetCharacter()->ShowScore(PlayerManager.CurrentScore, PlayerManager.UpgradeReloadSpeed());
	}
}

void UGameplayComponent::UpgradeWeapon()
{
	if (PlayerManager.CanUpgradeWeapon())
	{
		GetCharacter()->ShowScore(PlayerManager.CurrentScore, PlayerManager.UpgradeWeapon());
		GetCharacter()->ShowMaxWeaponProjectiles(PlayerManager.Weapon.MaxProjectiles);
	}
}

void UGameplayComponent::UpgradeProjectileCapacity()
{
	if (PlayerManager.CanUpgradeProjectileCapacity())
	{
		GetCharacter()->ShowScore(PlayerManager.CurrentScore, PlayerManager.UpgradeProjectileCapacity());
		GetCharacter()->ShowMaxPlayerProjectiles(PlayerManager.Player.MaxProjectiles);
	}
}

bool UGameplayComponent::CanResupply() const
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(ResupplyTimerHandle) <= 0.0f and PlayerManager.CanResupply();
}

void UGameplayComponent::Resupply()
{
	//TODO cost (inc with level?)
	if (const float TimerRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(ResupplyTimerHandle); TimerRemaining <= 0.0f)
	{
		PlayerManager.ResupplyProjectiles();
		GetCharacter()->ShowPlayerProjectiles(PlayerManager.Player.Projectiles);
		GetWorld()->GetTimerManager().ClearTimer(ResupplyTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(ResupplyTimerHandle, 60.0f, false);
	}
	else
	{
		GetCharacter()->ShowResupplyCooldown(TimerRemaining);
	}
}

ALttnCharacter* UGameplayComponent::GetCharacter()
{
	if (!Character)
	{
		Character = Cast<ALttnCharacter>(GetOwner());
	}

	return Character;
}

void UGameplayComponent::BeginHealthRegen()
{
	GetWorld()->GetTimerManager().ClearTimer(HealthCooldownTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(HealthRegenTimerHandle,
	                                       this, &UGameplayComponent::IncrementHealth,
	                                       1 / PlayerManager.Player.HealthRegenRate, true);
}

void UGameplayComponent::IncrementHealth()
{
	if (PlayerManager.Player.Health < PlayerManager.Player.MaxHealth)
	{
		GetCharacter()->SetPlayerHealth(PlayerManager.IncrementPlayerHealth());
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
	}
}

void UGameplayComponent::StartedFiring()
{
	bFireRateCooldownActive = false;
	if (bShouldStopFiring)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);
		bShouldStopFiring = false;
	}
	else
	{
		TryFire();
	}
}

void UGameplayComponent::TryFire()
{
	if (bIsReloading)
	{
		StopFiring();
		return;
	}
	if (PlayerManager.CanFire())
	{
		Fire();

		GetCharacter()->UpdateWeaponProjectiles(PlayerManager.DecrementProjectileCount()); // todo this is for the HUD


		if (!PlayerManager.ShouldReload())
		{
			StopFiring();
			Reload();
		}
	}
	else
	{
		StopFiring();
	}
}

void UGameplayComponent::Fire()
{
	const APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	const FVector Start = PlayerCameraManager->GetCameraLocation();
	const FWeaponInfo Weapon = PlayerManager.Weapon;
	const FVector RotatedCameraForwardVector = PlayerCameraManager->GetCameraRotation().Vector() * Weapon.Range;
	const FVector End = FVector(RotatedCameraForwardVector.X + Start.X,
	                            RotatedCameraForwardVector.Y + Start.Y,
	                            RotatedCameraForwardVector.Z + Start.Z);

	TArray<FHitResult> UnUniqueHits;
	GetWorld()->LineTraceMultiByChannel(UnUniqueHits, Start, End, ECC_Visibility);
	TArray<FHitResult> Hits = SortHits(UnUniqueHits, Weapon.Penetration);
	TArray<FHitResult> BlockingHits;

	for (FHitResult Hit : Hits)
	{
		if (AActor* Actor = Hit.GetActor(); Actor and Actor->IsA(ABotCharacter::StaticClass()))
		{
			ABotCharacter* Bot = Cast<ABotCharacter>(Actor);

			const bool bHead = Hit.BoneName == FName("head") or Hit.BoneName == FName("neck_01") or
				Hit.BoneName == FName("neck_02");

			float Damage = Weapon.Damage;
			if (bHead)
			{
				Damage *= 2;
			}

			// Server_DoFire(Bot, Damage); 
			// TODO is this ok or does it need to be done on server? Or is it automatically replicated. Should bot health be replicated instead?
			UGameplayStatics::ApplyDamage(Bot, Damage, GetOwner()->GetInstigatorController(), GetOwner(), UDamageType::StaticClass());

			int32 Score = 10;
			if (bHead)
			{
				Score = 25;
			}

			const int32 UpdatedScore = PlayerManager.AddScore(Score);

			GetCharacter()->ShowScore(UpdatedScore, Score);
		}

		if (Hit.bBlockingHit)
		{
			BlockingHits.Add(Hit);
		}
	}

	if (!BlockingHits.IsEmpty())
	{
		UE_LOG(LogHAL, Log, TEXT("UGameplayComponent::Fire had blocking hit"));
		DidFire(BlockingHits[0].Location);
	}
	else
	{
		UE_LOG(LogHAL, Log, TEXT("UGameplayComponent::Fire NO blocking hit"));
		DidFire(End);
	}
}

TArray<FHitResult> UGameplayComponent::SortHits(TArray<FHitResult> Array, const int32 Penetration)
{
	TMap<AActor*, FHitResult> ActorHits;

	for (FHitResult Hit : Array)
	{
		if (!Hit.GetActor()->IsA(ALttnCharacter::StaticClass()))
		{
			if (ActorHits.Contains(Hit.GetActor()))
			{
				if (Hit.BoneName == FName("head") or Hit.BoneName == FName("neck_01") or
					Hit.BoneName == FName("neck_02"))
				{
					ActorHits[Hit.GetActor()] = Hit;
				}
			}
			else
			{
				ActorHits.Add(Hit.GetActor(), Hit);
			}
			if (ActorHits.Num() == Penetration)
			{
				TArray<FHitResult> Hits;
				ActorHits.GenerateValueArray(Hits);

				return Hits;
			}
		}
	}
	TArray<FHitResult> Hits;
	ActorHits.GenerateValueArray(Hits);

	return Hits;
}

void UGameplayComponent::DidFire(const FVector& HitLocation)
{
	Server_DidFire(GetCharacter()->GetWeaponLocation(), HitLocation);
}

void UGameplayComponent::Server_DidFire_Implementation(const FVector& WeaponLocation, const FVector& HitLocation)
{
	Multi_DidFire(WeaponLocation, HitLocation);
}

void UGameplayComponent::Multi_DidFire_Implementation(const FVector& WeaponLocation, const FVector& HitLocation)
{
	UNiagaraComponent* SpawnedSystem = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		FireSystem,
		WeaponLocation,
		FRotator::ZeroRotator,
		FVector(1.0f),
		true,
		true,
		ENCPoolMethod::None,
		true
	);

	SpawnedSystem->SetVectorParameter(FName("Start"), WeaponLocation);
	SpawnedSystem->SetVectorParameter(FName("Target"), HitLocation);
}

void UGameplayComponent::ReloadComplete()
{
	PlayerManager.Reload();
	GetCharacter()->PlayerReloaded(PlayerManager.Weapon.ProjectileCount, PlayerManager.Player.Projectiles);
	bIsReloading = false;
}

void UGameplayComponent::DecrementStamina()
{
	if (!bIsMoving)
	{
		return;
	}
	if (PlayerManager.Player.Stamina > 0)
	{
		if (const float PlayerStamina = PlayerManager.UpdateStamina(-1); PlayerStamina <= 0)
		{
			GetCharacter()->StaminaDepleted();
		}
		else
		{
			GetCharacter()->SetPlayerStamina(PlayerStamina / PlayerManager.Player.MaxStamina);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaDegenTimerHandle);
		StaminaDepletedDelegate.Broadcast();
	}
}

void UGameplayComponent::IncrementStamina()
{
	if (PlayerManager.Player.Stamina < PlayerManager.Player.MaxStamina)
	{
		GetCharacter()->SetPlayerStamina(PlayerManager.UpdateStamina(1) / PlayerManager.Player.MaxStamina);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
}
