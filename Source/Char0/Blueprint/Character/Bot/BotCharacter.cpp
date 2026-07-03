#include "BotCharacter.h"

#include "BrainComponent.h"
#include "Char0/Blueprint/Character/LttnCharacter.h"
#include "Char0/Blueprint/Controller/LttnController.h"
#include "Char0/Blueprint/Controller/Bot/BotAiController.h"
#include "Char0/Blueprint/GameMode/LttnGameMode.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ABotCharacter::ABotCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ABotCharacter::Activate(const int32 Level)
{
	MC_Activate(Level);
	Int_Activate(Level);
}

void ABotCharacter::BeginPlay()
{
	MaxHealth = 2.0f;
	if (IsLocallyControlled())
	{
		SetHealth(MaxHealth);
	}
	// SpawnTransform = GetTransform();
	CapsuleCollision = GetCapsuleComponent()->GetCollisionEnabled();
	MeshCollision = GetMesh()->GetCollisionEnabled();
	LttnGameMode = Cast<ALttnGameMode>(GetWorld()->GetAuthGameMode());
	Super::BeginPlay();
}

float ABotCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!bIsDying and EventInstigator->IsA(ALttnController::StaticClass()))
	{
		const ALttnCharacter* CombatCharacter = Cast<ALttnCharacter>(DamageCauser);
		const ALttnController* CombatPlayerController = Cast<ALttnController>(CombatCharacter->Controller); //todo this isn't the best?

		LastDamagedPlayerId = CombatPlayerController->Id;
		const float DamageApplied = Health - Damage;
		SetHealth(DamageApplied);
		return DamageApplied;
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

float ABotCharacter::Attack()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		float AnimDuration = AnimInstance->Montage_Play(AttackAnim);
		//TODO start sphere trace for attack -> also attack right or left
		return AnimDuration;
	}
	else
	{
		return 0.0f;
	}
}

void ABotCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
                              const FHitResult& Hit)
{
	//TODO instead of this, do sphere trace around hands for duration of attack anim
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if (!bIsDying and Other->IsA(ALttnCharacter::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(Other, HitDamage, GetInstigator()->Controller, this, UDamageType::StaticClass());
	}
}

void ABotCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABotCharacter, Health);
	DOREPLIFETIME(ABotCharacter, MaxHealth);
	DOREPLIFETIME(ABotCharacter, HitDamage);
}

void ABotCharacter::MC_Activate_Implementation(const int32 Level)
{
	Int_Activate(Level);
}

void ABotCharacter::Int_Activate(const int32 Level)
{
	SetLevel(Level);
	if (HasAuthority())
	{
		SetEnabled(true);
	}
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->StopMovementImmediately();
	}
}

void ABotCharacter::SetLevel(const int32 CurrentLevel)
{
	MaxHealth = (CurrentLevel + 1) * 2;
	HitDamage = CurrentLevel + 1;
	SetHealth(MaxHealth);
}

void ABotCharacter::SetEnabled(const bool bEnabled)
{
	if (!HasAuthority())
	{
		return;
	}
	UCapsuleComponent* Capsule = GetCapsuleComponent();

	GetCharacterMovement()->bSweepWhileNavWalking = bEnabled;
	Capsule->SetGenerateOverlapEvents(bEnabled);
	SetActorEnableCollision(bEnabled);
	SetActorHiddenInGame(!bEnabled);
	SetActorTickEnabled(bEnabled);
	bIsDying = !bEnabled;
	SetDead(!bEnabled);

	if (bEnabled)
	{
		Capsule->SetCollisionEnabled(CapsuleCollision);
		GetMesh()->SetCollisionEnabled(MeshCollision);

		// GetCharacterMovement()->DisableMovement();
		// GetMesh()->SetSimulatePhysics(false);
		if (!bIsPossessed)
		{
			ABotAiController* BotAiController = GetAiController();
			BotAiController->Possess(this);
			BotAiController->EnableStateTree();
			bIsPossessed = true;
			BotAiController->BrainComponent->StartLogic();
		}
	}
	else
	{
		ABotAiController* BotAiController = GetAiController();
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// GetCharacterMovement()->DisableMovement();//TODO this or just stop
		// GetMesh()->SetSimulatePhysics(true);
		BotAiController->BrainComponent->StopLogic("Bot is dead");
		if (bIsPossessed)
		{
			BotAiController->DisableStateTree();
			BotAiController->UnPossess();
			bIsPossessed = false;
		}
	}
}

void ABotCharacter::SetHealth(const float Value)
{
	if (HasAuthority())
	{
		SetHealthBar(Value / MaxHealth);
		Health = FMath::Clamp<float>(Value, 0.0f, MaxHealth);
		OnHealthUpdated();
	}
}

void ABotCharacter::OnRep_Health()
{
	OnHealthUpdated();
}

void ABotCharacter::OnHealthUpdated()
{
	if (Health <= 0)
	{
		SetEnabled(false);
		// Dead(LastDamagedPlayerId);
		bIsDying = true;
		SetDead(true);
		LttnGameMode->DecrementBots(LastDamagedPlayerId);
	}
}

ABotAiController* ABotCharacter::GetAiController()
{
	if (AiController == nullptr)
	{
		AiController = Cast<ABotAiController>(GetController());
	}
	return AiController;
}
