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

};
