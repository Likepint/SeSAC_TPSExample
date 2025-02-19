#include "Characters/CEnemyAnimInstance.h"
#include "Characters/CEnemy.h"

void UCEnemyAnimInstance::AnimNotify_AttackEnd()
{
	if (ACEnemy* enemy = Cast<ACEnemy>(TryGetPawnOwner()))
		enemy->FSM->OnAttackEnd();

}

// �Ѿ����� �ִϸ��̼��� ������ ��� ȣ��
void UCEnemyAnimInstance::AnimNotify_DeadEnd()
{
	if (ACEnemy* enemy = Cast<ACEnemy>(TryGetPawnOwner()))
		enemy->FSM->OnDeadEnd();

}
