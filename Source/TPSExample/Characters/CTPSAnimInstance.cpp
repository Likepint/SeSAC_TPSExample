#include "Characters/CTPSAnimInstance.h"
#include "Characters/CTPSCharacter.h"

void UCTPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (ACTPSCharacter* player = Cast<ACTPSCharacter>(TryGetPawnOwner()))
	{
		FVector velocity = player->GetVelocity();
		FVector forwardVector = player->GetActorForwardVector();

		// 앞 / 뒤 이동속도
		Speed = FVector::DotProduct(velocity, forwardVector);
	}
}
