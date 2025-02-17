#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CEnemyFSMComponent.generated.h"

// 사용할 상태 정의
UENUM ( BlueprintType )
enum class EEnemyState : uint8
{
	Idle = 0 UMETA ( DisplayName = "대기" ), Move , Attack , Damaged , Dead , MAX
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSEXAMPLE_API UCEnemyFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCEnemyFSMComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
public:
	// 상태 변수
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSM")
	EEnemyState mState = EEnemyState::Idle;

	/* 각 상태에 대한 함수 */
	// 대기 상태
	void IdleState();
	// 이동 상태
	void MoveState();
	// 공격 상태
	void AttackState();
	// 피격 상태
	void DamagedState();
	// 사망 상태
	void DeadState();

	// 대기 상태에서 쓸 변수
	// 경과 시간
	UPROPERTY(EditAnywhere , Category = "FSM")
	float CurrentTime = 0;
	// 대기 시간
	UPROPERTY(EditAnywhere , Category = "FSM")
	float IdleDelayTime = 2;

	// 타겟
	UPROPERTY(VisibleAnywhere, Category = "FSM")
	class ACTPSCharacter* Target;

	// 소유하고 있는 오너
	UPROPERTY()
	class ACEnemy* me;

	// 공격 범위
	UPROPERTY(VisibleAnywhere, Category = "FSM")
	float AttackRange = 200;

	// 공격 대기 시간
	UPROPERTY(VisibleAnywhere, Category = "FSM")
	float AttackDelayTime = 2;

	// 피격 알림 이벤트 함수
	void OnDamageProcess();

	// 체력
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	int32 hp = 3;

	// 피격 대기 시간
	UPROPERTY(VisibleAnywhere, Category = "FSM")
	float DamageDelayTime = 2;

	UPROPERTY(VisibleAnywhere, Category = "FSM")
	float DeadSpeed = 50;

	UPROPERTY()
	class UCEnemyAnimInstance* Anim;

	void OnAttackEnd();

};
