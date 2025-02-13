#include "Characters/CTPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Weapons/CBullet.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CEnemyFSMComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Characters/CTPSAnimInstance.h"

ACTPSCharacter::ACTPSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	InitializeCharacter();

}

void ACTPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* controller = Cast<APlayerController>(GetController()))
	{	// Get local player subsystem
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer()))
			// Add input context
			Subsystem->AddMappingContext(IMC_TPS, 0);
	}

	// 무기 모두 숨김 처리
	RifleMesh->SetVisibility(false);
	SniperMesh->SetVisibility(false);

	// 스나이퍼 UI 위젯 인스턴스 생성
	SniperUI = CreateWidget<UUserWidget>(GetWorld(), SniperUIFactory);

	// 일반 조준 크로스헤어UI 위젯 인스턴스 생성
	CrossHairUI = CreateWidget<UUserWidget>(GetWorld(), CrosshairUIFactory);

	// 일반 조준 UI 등록
	CrossHairUI->AddToViewport();

	// 초기속도를 걷기로 설정
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ACTPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACTPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_VerticalLook, ETriggerEvent::Triggered, this, &ACTPSCharacter::OnVerticalLook);
		EnhancedInputComponent->BindAction(IA_HorizontalLook, ETriggerEvent::Triggered, this, &ACTPSCharacter::OnHorizontalLook);

		EnhancedInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &ACTPSCharacter::OnMovement);

		EnhancedInputComponent->BindAction(IA_JumpAction, ETriggerEvent::Started, this, &ACTPSCharacter::OnJumpAction);

		EnhancedInputComponent->BindAction(IA_Rifle, ETriggerEvent::Started, this, &ACTPSCharacter::OnRifle);
		EnhancedInputComponent->BindAction(IA_Sniper, ETriggerEvent::Started, this, &ACTPSCharacter::OnSniper);

		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &ACTPSCharacter::OnFire);

		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Started, this, &ACTPSCharacter::OnZoom);
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Completed, this, &ACTPSCharacter::OnZoom);

		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Triggered, this, &ACTPSCharacter::OnRun);
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Completed, this, &ACTPSCharacter::OnRun);

	}
}

void ACTPSCharacter::OnVerticalLook(const FInputActionValue& InVal)
{
	float value = InVal.Get<float>();

	AddControllerPitchInput(value);
	
}

void ACTPSCharacter::OnHorizontalLook(const FInputActionValue& InVal)
{
	float value = InVal.Get<float>();

	AddControllerYawInput(value);

}

void ACTPSCharacter::OnMovement(const FInputActionValue& InVal)
{
	// Forward
	AddMovementInput(GetActorForwardVector(), InVal.Get<FVector>().X);

	// Right
	AddMovementInput(GetActorRightVector(), InVal.Get<FVector>().Y);

}

void ACTPSCharacter::OnJumpAction(const FInputActionValue& InVal)
{
	Jump();

}

void ACTPSCharacter::OnRifle(const FInputActionValue& InVal)
{
	// 라이플이면 TRUE, 스나이퍼면 FALSE
	bRifle = true;

	if (SniperMesh->IsVisible())
		SniperMesh->SetVisibility(false);

	RifleMesh->SetVisibility(true);
}

void ACTPSCharacter::OnSniper(const FInputActionValue& InVal)
{
	// 라이플이면 TRUE, 스나이퍼면 FALSE
	bRifle = false;

	if (RifleMesh->IsVisible())
		RifleMesh->SetVisibility(false);

	SniperMesh->SetVisibility(true);
}

void ACTPSCharacter::OnFire(const FInputActionValue& InVal)
{
	// 공격 애니메이션 재생
	if (auto anim = Cast<UCTPSAnimInstance>(GetMesh()->GetAnimInstance()))
		anim->PlayAttackAnim();

	// Rifle
	if (bRifle)
	{
		FTransform firePosition = RifleMesh->GetSocketTransform ( "FirePosition" );

		GetWorld ( )->SpawnActor<ACBullet> ( BulletFactory , firePosition );
	}
	// Sniper
	else
	{
		// LineTrace의 시작 위치
		FVector start = Camera->GetComponentLocation();
		 
		// LineTrace의 종료 위치
		FVector end = start + Camera->GetForwardVector() * 5000;
		
		// LineTrace의 충돌 정보를 담을 변수
		FHitResult info;

		// 충돌 옵션 설정 변수
		FCollisionQueryParams params;

		// 플레이어 제외 설정
		params.AddIgnoredActor(this);

		// Channel 필터를 이용한 LineTrace 충돌 검출
		bool bHit = GetWorld()->LineTraceSingleByChannel(info, start, end, ECollisionChannel::ECC_Visibility, params);

		// LineTrace가 충돌했을 때
		if (bHit)
		{
			// 충돌 처리 -> 충돌효과 표현
			
			// 총알 파편 효과 출력할 트랜스폼
			FTransform t;

			// 부딪힌 위치 할당
			t.SetLocation(info.ImpactPoint);

			// 총알 파편 효과 인스턴스 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletEffectFactory, t);

			// 충돌한 오브젝트의 컴포넌트
			auto hitComp = info.GetComponent();

			// 해당 컴포넌에 물리가 적용되어 있다면
			if (hitComp and hitComp->IsSimulatingPhysics())
			{
				// 조준한 방향
				FVector direction = (end - start).GetSafeNormal();

				// 날려버릴 힘 ( F = ma )
				FVector force = direction * hitComp->GetMass() * 500000;

				// 그 방향으로 날리기
				hitComp->AddForceAtLocation(force, info.ImpactPoint);
			}

			// 부딪힌 대상이 적인 체크
			auto enemy = info.GetActor()->GetDefaultSubobjectByName("FSM");

			if (enemy)
			{
				auto enemyFSM = Cast<UCEnemyFSMComponent>(enemy);
				enemyFSM->OnDamageProcess();
			}
		}
	}
}

void ACTPSCharacter::OnZoom(const FInputActionValue& InVal)
{
	// 스나이퍼건 모드가 아니라면 처리X
	if (bRifle) return;
	
	if (bOnZoom == false)
	{// Started 입력처리
		bOnZoom = true;

		SniperUI->AddToViewport();

		// 카메라의 시야각 FOV 설정
		Camera->SetFieldOfView(45);

		CrossHairUI->RemoveFromParent();
	}
	else
	{// Completed 입력처리
		bOnZoom = false;

		SniperUI->RemoveFromParent();

		Camera->SetFieldOfView(90);

		CrossHairUI->AddToViewport();
	}

}

void ACTPSCharacter::OnRun(const struct FInputActionValue& InVal)
{
	if (InVal.Get<bool>())
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	else GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ACTPSCharacter::InitializeCharacter()
{
	// Mesh에 SK_Mannequin 로드 후 설정
	ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(L"/Script/Engine.SkeletalMesh'/Game/PJS/Characters/Meshes/SK_Mannequin.SK_Mannequin'");
	if (mesh.Succeeded()) // 성공이라면
		GetMesh()->SetSkeletalMesh(mesh.Object);

	// 위치값과 회전값 반영
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));

	bUseControllerRotationYaw = true;

	// SpringArm Component 생성
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	// Root Component에 Attach
	SpringArm->SetupAttachment(RootComponent);
	// 상대위치 지정
	SpringArm->SetRelativeLocation(FVector(0, 60, 80));
	// TargetArmLength 지정
	SpringArm->TargetArmLength = 300;
	// UsePawnControlRotation 설정
	SpringArm->bUsePawnControlRotation = true;

	// Camera Component 생성
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	// SpringArm Component에 Attach
	Camera->SetupAttachment(SpringArm);
	// UsePawnControlRotation 설정
	Camera->bUsePawnControlRotation = false;

	// 라이플 스켈레탈 메시 컴포넌트 등록
	RifleMesh = CreateDefaultSubobject<USkeletalMeshComponent>("RifleMesh");

	// 스켈레탈 메시 위치 지정
	RifleMesh->SetRelativeLocation(FVector(0, 60, 120));

	// 부모 컴포넌트를 Mesh 컴포넌트로 설정
	RifleMesh->SetupAttachment(GetMesh());

	// 스켈레탈 메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> rifle(L"/Script/Engine.SkeletalMesh'/Game/PJS/Weapons/Rifle/Mesh/SK_FPGun.SK_FPGun'");
	if (rifle.Succeeded())
		RifleMesh->SetSkeletalMesh(rifle.Object);

	// 스나이퍼 스태틱 메시 컴포넌트 등록
	SniperMesh = CreateDefaultSubobject<UStaticMeshComponent>("SniperMesh");

	// 부모 컴포넌트를 Mesh 컴포넌트로 설정
	SniperMesh->SetupAttachment(GetMesh());

	// 스나이퍼 메시 위치 지정
	SniperMesh->SetRelativeLocation(FVector(0, 90, 120));

	// 스나이퍼 메시 스케일 지정
	SniperMesh->SetRelativeScale3D(FVector(0.15));

	ConstructorHelpers::FObjectFinder<UStaticMesh> sniper(L"/Script/Engine.StaticMesh'/Game/PJS/Weapons/Sniper/Mesh/sniper.sniper'");
	if (sniper.Succeeded())
		SniperMesh->SetStaticMesh(sniper.Object);

}
