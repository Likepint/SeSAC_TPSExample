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

	UFUNCTION()
	void AnimNotify_AttackEnd();
};
