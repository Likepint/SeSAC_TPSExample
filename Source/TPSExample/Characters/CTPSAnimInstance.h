#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CTPSAnimInstance.generated.h"

UCLASS()
class TPSEXAMPLE_API UCTPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 매 프레임 갱신되는 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "TPS")
	float Speed = 0;

	// 플레이어 좌우 이동속도
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "TPS")
	float Direction = 0;

	// 공중에 있는 상태인지 확인할 Bool변수
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "TPS")
	bool IsInAir = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "TPS")
	bool bCrouched = false;

	// 재생할 공격 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "TPS")
	class UAnimMontage* AttackAnimMontage;

public:
	void PlayAttackAnim();

};
