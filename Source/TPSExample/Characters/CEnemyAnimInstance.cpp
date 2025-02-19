#include "Characters/CEnemyAnimInstance.h"
#include "Characters/CEnemy.h"

void UCEnemyAnimInstance::AnimNotify_AttackEnd()
{
	if (ACEnemy* enemy = Cast<ACEnemy>(TryGetPawnOwner()))
		enemy->FSM->OnAttackEnd();

}

// 넘어지는 애니메이션이 끝나는 경우 호출
void UCEnemyAnimInstance::AnimNotify_DeadEnd()
{
	if (ACEnemy* enemy = Cast<ACEnemy>(TryGetPawnOwner()))
		enemy->FSM->OnDeadEnd();

}
