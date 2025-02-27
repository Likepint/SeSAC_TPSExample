#include "Components/CEnemyFSMComponent.h"
#include "TPSExample.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CTPSCharacter.h"
#include "Characters/CEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Characters/CEnemyAnimInstance.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

UCEnemyFSMComponent::UCEnemyFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCEnemyFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	// 월드에서 ATPSCharacter 찾기
	if (auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ACTPSCharacter::StaticClass()))
	{
		// ACTPSCharacter 캐스팅
		Target = Cast<ACTPSCharacter>(actor);

		// 오너 불러오기
		me = Cast<ACEnemy>(GetOwner());

		Anim = Cast<UCEnemyAnimInstance>(me->GetMesh()->GetAnimInstance());

		AI = Cast<AAIController>(me->GetController());
	}
}

void UCEnemyFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 실행창에 상태 메세지 출력
	FString msg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, msg);

	switch (mState)
	{
		case EEnemyState::Idle:
		{
			IdleState();

			break;
		}
		case EEnemyState::Move:
		{
			MoveState();

			break;
		}
		case EEnemyState::Attack:
		{
			AttackState();

			break;
		}
		case EEnemyState::Damaged:
		{
			DamagedState();

			break;
		}
		case EEnemyState::Dead:
		{
			DeadState();

			break;
		}
	}
}

void UCEnemyFSMComponent::IdleState()
{
	// 시간이 흐르도록 설정
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	// 만약 경과 시간이 대기시간을 초과했다면
	if (CurrentTime >= IdleDelayTime)
	{
		// 이동 상태로 전환
		mState = EEnemyState::Move;

		Anim->AnimState = mState;

		// 경과 시간 초기화
		CurrentTime = 0;

		// 새로운 랜덤 위치 가져오기
		GetRandPositionInNavMesh(me->GetActorLocation(), 500, RandPosition);
	}
}

void UCEnemyFSMComponent::MoveState()
{
	// 타겟 목적지 필요
	FVector dest = Target->GetActorLocation();

	// 방향 설정
	FVector dir = dest - me->GetActorLocation();

	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	// 목적지 길 찾기 경로 데이터 검색
	FPathFindingQuery query;

	FAIMoveRequest request;

	// 목적지에서 인지할 수 있는 타입
	request.SetAcceptanceRadius(3);
	request.SetGoalLocation(dest);

	// 길 찾기를 위한 쿼리 생성
	AI->BuildPathfindingQuery(request, query);

	// 길 찾기 결과를 로드
	FPathFindingResult r = ns->FindPathSync(query);

	// 목적지까지 길 찾기 성공 여부 확인
	if (r.Result == ENavigationQueryResult::Success)
	{
		// 타겟으로 이동
		AI->MoveToLocation(dest);
	}
	else
	{
		// 랜덤한 위치로 이동
		auto result = AI->MoveToLocation(RandPosition);

		// 목적지에 도착하면
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			// 새로운 랜덤 위치 가져오기
			GetRandPositionInNavMesh(me->GetActorLocation(), 500, RandPosition);
		}
	}

	// dir 방향으로 캐릭터 이동
	//me->AddMovementInput(dir.GetSafeNormal());
	//AI->MoveToLocation(dest);

	// 타겟과 거리를 체크해서 AttackRange 안으로 들어오면 공격 상태로 전환
	// 거리 체크
	if (dir.Size() <= AttackRange)
	{
		// 길 찾기 기능 정지
		AI->StopMovement();

		mState = EEnemyState::Attack;

		Anim->AnimState = mState;

		Anim->bAttackPlay = true;

		CurrentTime = AttackDelayTime;
	}
}

void UCEnemyFSMComponent::AttackState()
{
	// 일정 시간에 한번씩 공격
	// 시간이 흐르다가
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	// 공격시간이 되면
	if (CurrentTime >= AttackDelayTime)
	{
		// 공격 실행
		PRINT_LOG(TEXT("Attack~!~!~!"));

		Anim->bAttackPlay = true;

		// 결과 시간 초기화
		CurrentTime = 0;
	}

	// 타겟이 공격범위를 벗어나면 이동상태로 전환
	// 타겟과의 거리
	float dist = FVector::Distance(Target->GetActorLocation(), me->GetActorLocation());

	// 타겟과의 거리가 공격 범위를 벗어나면
	if (dist > AttackRange)
	{
		mState = EEnemyState::Move;

		// 새로운 랜덤 위치 가져오기
		GetRandPositionInNavMesh(me->GetActorLocation(), 500, RandPosition);
	}
}

void UCEnemyFSMComponent::DamagedState()
{
	// 시간이 흐르다가
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	// 경과 시간이 대기 시간을 초과하면
	if (CurrentTime >= DamageDelayTime)
	{
		// 대기 상태로 전환
		mState = EEnemyState::Idle;

		Anim->AnimState = mState;

		// 경과 시간 초기화
		CurrentTime = 0;
	}
}

void UCEnemyFSMComponent::DeadState()
{
	if (bDead == false) return;

	// 계속 아래로 내려가고 싶다.
	// P = P0 + vt;
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * DeadSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	// 만약 2미터 이상 내려왔다며 제거
	if (P.Z <= -200)
		me->Destroy();
}

void UCEnemyFSMComponent::OnDamageProcess(int32 InDamage)
{
	// 체력 감소
	hp -= 1;

	// 체력이 남아 있는지 체크
	if (hp > 0)
	{
		mState = EEnemyState::Damaged;

		int randValue = FMath::RandRange(0, 1);
		FString section = FString::Printf(L"Damage%d", randValue);

		me->PlayAnimMontage(Anim->EnemyMontage, 1, FName(*section));
	}
	else
	{
		mState = EEnemyState::Dead;

		// 캡슐의 충돌체 비활성화
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		me->PlayAnimMontage(Anim->EnemyMontage, 1, L"Dead");
	}

	// 길 찾기 기능 중단
	AI->StopMovement();

	Anim->AnimState = mState;
}

void UCEnemyFSMComponent::OnAttackEnd()
{
	Anim->bAttackPlay = false;
}

bool UCEnemyFSMComponent::GetRandPositionInNavMesh(FVector InCenterLocation, float InRadius, FVector& InDest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FNavLocation location;

	bool result = ns->GetRandomReachablePointInRadius(InCenterLocation, InRadius, location);

	InDest = location.Location;

	return result;
}
