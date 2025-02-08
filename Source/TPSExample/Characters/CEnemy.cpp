#include "Characters/CEnemy.h"
#include "Components/CEnemyFSMComponent.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	InitializeCharacter();
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACEnemy::InitializeCharacter()
{
	// Mesh에 SK_Mannequin 로드 후 설정
	ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(L"/Script/Engine.SkeletalMesh'/Game/PJS/Characters/Meshes/SK_Mannequin.SK_Mannequin'");
	if (mesh.Succeeded()) // 성공이라면
		GetMesh()->SetSkeletalMesh(mesh.Object);

	// 위치값과 회전값 반영
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));

	// EnemyFSM 컴포넌트 추가
	FSM = CreateDefaultSubobject<UCEnemyFSMComponent>("FSM");
}
