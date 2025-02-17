#include "Characters/CEnemyAnimInstance.h"
#include "Characters/CEnemy.h"

void UCEnemyAnimInstance::AnimNotify_AttackEnd()
{
	if (ACEnemy* enemy = Cast<ACEnemy>(TryGetPawnOwner()))
		enemy->FSM->OnAttackEnd();

}
