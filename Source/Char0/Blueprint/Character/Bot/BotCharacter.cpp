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
	MC_Attack();

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		const float AnimDuration = AnimInstance->Montage_Play(AttackAnim);
		StartWeaponTrace(AnimDuration);
		return AnimDuration;
	}
	return 0.0f;
}

void ABotCharacter::MC_Attack_Implementation()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		const float AnimDuration = AnimInstance->Montage_Play(AttackAnim);
		StartWeaponTrace(AnimDuration);
	}
}

void ABotCharacter::StartWeaponTrace(const float Duration)
{
	WeaponTraceDuration = Duration;
	WeaponTraceInterval = Duration / 250;
	GetWorldTimerManager().ClearTimer(WeaponTraceTimerHandle);
	GetWorldTimerManager().SetTimer(WeaponTraceTimerHandle, this, &ABotCharacter::DoWeaponTrace, WeaponTraceInterval, true);
}

void ABotCharacter::DoWeaponTrace()
{
	WeaponTraceCurrent += WeaponTraceInterval;

	if (WeaponTraceCurrent <= WeaponTraceInterval * 50) //wee delay so it doesn't hit on the back swing
	{
		return;
	}

	const bool bHitRight = AttackHitRight.GetActor() != nullptr and AttackHitRight.GetActor()->IsA(ALttnCharacter::StaticClass());
	const bool bHitLeft = AttackHitLeft.GetActor() != nullptr and AttackHitLeft.GetActor()->IsA(ALttnCharacter::StaticClass());

	if (WeaponTraceCurrent >= WeaponTraceDuration or bHitRight or bHitLeft)
	{
		AttackFinished();
		return;
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ToIgnore;
	ToIgnore.Add(this);

	if (bShouldAttackRight)
	{
		const FVector RightSocket = GetMesh()->GetSocketLocation(FName("hand_r"));

		FHitResult OutHit;
		const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), RightSocket, RightSocket, 10.0f, TraceObjectTypes, false, ToIgnore, EDrawDebugTrace::Type::ForDuration, OutHit,
		                                                                    true,
		                                                                    FLinearColor::Red, FLinearColor::Green, 1.0);

		if (bHit and OutHit.GetActor()->IsA(ALttnCharacter::StaticClass()))
		{
			AttackHitRight = OutHit;
			return;
		}
	}

	if (!bShouldAttackRight)
	{
		const FVector LeftSocket = GetMesh()->GetSocketLocation(FName("hand_l"));
		FHitResult OtherOutHit;
		const bool bAnotherHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), LeftSocket, LeftSocket, 10.0f, TraceObjectTypes, false, ToIgnore, EDrawDebugTrace::Type::ForDuration,
		                                                                           OtherOutHit, true, FLinearColor::Red, FLinearColor::Green, 1.0);
		if (bAnotherHit and OtherOutHit.GetActor()->IsA(ALttnCharacter::StaticClass()))
		{
			AttackHitLeft = OtherOutHit;
		}
	}
}

void ABotCharacter::AttackFinished()
{
	GetWorldTimerManager().ClearTimer(WeaponTraceTimerHandle);
	WeaponTraceCurrent = 0;
	WeaponTraceDuration = 0;
	WeaponTraceInterval = 0;

	if (AttackHitRight.GetActor() != nullptr and AttackHitRight.GetActor()->IsA(ALttnCharacter::StaticClass()))
	{
		UGameplayStatics::ApplyPointDamage(AttackHitRight.GetActor(), 1.0F/*HitDamage*/, AttackHitRight.ImpactPoint, AttackHitRight, GetController(), this, nullptr);
	}

	if (AttackHitLeft.GetActor() != nullptr and AttackHitLeft.GetActor()->IsA(ALttnCharacter::StaticClass()))
	{
		UGameplayStatics::ApplyPointDamage(AttackHitLeft.GetActor(), HitDamage, AttackHitLeft.ImpactPoint, AttackHitLeft, GetController(), this, nullptr);
	}

	AttackHitRight = FHitResult();
	AttackHitLeft = FHitResult();
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
		if (HasAuthority())
		{
			LttnGameMode->DecrementBots(LastDamagedPlayerId);
		}
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
