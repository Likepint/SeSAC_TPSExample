#include "Characters/CTPSAnimInstance.h"
#include "Characters/CTPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCTPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (ACTPSCharacter* player = Cast<ACTPSCharacter>(TryGetPawnOwner()))
	{
		FVector velocity = player->GetVelocity();
		FVector forwardVector = player->GetActorForwardVector();
		FVector rightVector = player->GetActorRightVector();

		// �� / �� �̵��ӵ�
		Speed = FVector::DotProduct(velocity, forwardVector);

		// �� / �� �̵��ӵ�
		Direction = FVector::DotProduct(velocity, rightVector);

		// �÷��̾ ���� ���߿� �ִ� �������� ����
		IsInAir = player->GetCharacterMovement()->IsFalling();
	}
}

void UCTPSAnimInstance::PlayAttackAnim()
{
	if (AttackAnimMontage)
		Montage_Play(AttackAnimMontage);
}
