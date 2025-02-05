#include "Weapons/CBullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACBullet::ACBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체 등록
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");

	// 충돌 프로파일 설정
	SphereCollision->SetCollisionProfileName("BlockAll");

	// 충돌체 크기 설정
	SphereCollision->SetSphereRadius(20);

	// 루트컴포넌트로 설정
	SetRootComponent(SphereCollision);

	// Static(외관) 컴포넌트 등록
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	// 부모 컴포넌트 지정
	StaticMesh->SetupAttachment(RootComponent);

	// 충돌 비활성화
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 외관 크기 설정
	StaticMesh->SetRelativeScale3D(FVector(0.35));

	// 외관 위치 설정
	StaticMesh->SetRelativeLocation(FVector(0, 0, -17));

	// 발사체 컴포넌트 등록
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>("Porjectile");

	// 컴포넌트가 갱신시킬 컴포넌트 지정
	Projectile->SetUpdatedComponent(SphereCollision);

	// 초기 속도
	Projectile->InitialSpeed = 5000;

	// 최대 속도
	Projectile->MaxSpeed = 5000;

	// 반동 여부
	Projectile->bShouldBounce = true;

	// 반동 값
	Projectile->Bounciness = 0.3;

	// 생명 시간 주기
	//InitialLifeSpan = 2;

}

void ACBullet::BeginPlay()
{
	Super::BeginPlay();
	
	//FTimerHandle deathTimer;
	////                                     타이머핸들,  객체, 바인딩할 함수, 주기, 반복여부
	//GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ACBullet::Die, 2, false);

	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer
	(deathTimer,
		FTimerDelegate::CreateLambda // 람다
		(
			[this]()->void // 리턴타입
			{
				Destroy(); // 함수의 구현부
			}
		),
	2, false);

}

void ACBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACBullet::Die()
{
	Destroy();

}
