#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CTPSAnimInstance.generated.h"

UCLASS()
class TPSEXAMPLE_API UCTPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// �� ������ ���ŵǴ� �Լ�
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "TPS")
	float Speed = 0;

};
