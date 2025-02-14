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

	// �÷��̾� �¿� �̵��ӵ�
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "TPS")
	float Direction = 0;

	// ���߿� �ִ� �������� Ȯ���� Bool����
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "TPS")
	bool IsInAir = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "TPS")
	bool bCrouched = false;

	// ����� ���� �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = "TPS")
	class UAnimMontage* AttackAnimMontage;

public:
	void PlayAttackAnim();

};
