#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CEnemyFSMComponent.h"
#include "CEnemyAnimInstance.generated.h"

UCLASS()
class TPSEXAMPLE_API UCEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	EEnemyState AnimState = EEnemyState::Idle;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	bool bAttackPlay = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	class UAnimMontage* EnemyMontage;

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_DeadEnd();
};
