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

		// 앞 / 뒤 이동속도
		Speed = FVector::DotProduct(velocity, forwardVector);

		// 좌 / 우 이동속도
		Direction = FVector::DotProduct(velocity, rightVector);

		// 플레이어가 현재 공중에 있는 상태인지 저장
		IsInAir = player->GetCharacterMovement()->IsFalling();
	}
}

void UCTPSAnimInstance::PlayAttackAnim()
{
	if (AttackAnimMontage)
		Montage_Play(AttackAnimMontage);
}
