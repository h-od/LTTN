#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BotCharacter.generated.h"

class ALttnGameMode;
class ABotAiController;

UCLASS()
class CHAR0_API ABotCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(ReplicatedUsing=OnRep_Health)
	float Health;
	UPROPERTY(Replicated)
	float MaxHealth;
	UPROPERTY(Replicated)
	float HitDamage;
	UPROPERTY()
	int32 LastDamagedPlayerId;
	UPROPERTY()
	bool bIsDying;
	
	UPROPERTY()
	FTimerHandle WeaponTraceTimerHandle;
	UPROPERTY()
	float WeaponTraceDuration;
	UPROPERTY()
	float WeaponTraceInterval;
	UPROPERTY()
	float WeaponTraceCurrent;
	UPROPERTY()
	bool bShouldAttackRight = true;

	UPROPERTY()
	FHitResult AttackHitRight;
	UPROPERTY()
	FHitResult AttackHitLeft;

	UPROPERTY()
	TEnumAsByte<ECollisionEnabled::Type> CapsuleCollision;
	UPROPERTY()
	TEnumAsByte<ECollisionEnabled::Type> MeshCollision;

	UPROPERTY()
	ALttnGameMode* LttnGameMode;
	UPROPERTY()
	ABotAiController* AiController;

	UPROPERTY()
	bool bIsPossessed = true;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackAnim;

public:
	ABotCharacter();

	void Activate(int32 Level);

	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	float Attack();
	UFUNCTION(NetMulticast, Reliable)
	void MC_Attack();
	void MC_Attack_Implementation();
	void StartWeaponTrace(float Duration);
	void DoWeaponTrace();
	void AttackFinished();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION(NetMulticast, Reliable)
	void MC_Activate(int32 Level);
	void MC_Activate_Implementation(int32 Level);
	void Int_Activate(int32 Level);
	void SetLevel(int32 CurrentLevel);
	void SetEnabled(bool bEnabled);
	void SetHealth(float Value);

	UFUNCTION()
	void OnRep_Health();

	void OnHealthUpdated();
	ABotAiController* GetAiController();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetHealthBar(float UpdatedHealth);
	UFUNCTION(BlueprintImplementableEvent)
	void Dead(int32 KilledByPlayerId);
	UFUNCTION(BlueprintImplementableEvent)
	void SetDead(int32 KilledByPlayerId);
};
