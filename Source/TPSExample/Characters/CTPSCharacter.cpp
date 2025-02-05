#include "Characters/CTPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Weapons/CBullet.h"

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

		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &ACTPSCharacter::OnFire);

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

void ACTPSCharacter::OnFire(const FInputActionValue& InVal)
{
	FTransform firePosition = RifleMesh->GetSocketTransform("FirePosition");

	GetWorld()->SpawnActor<ACBullet>(BulletFactory, firePosition);

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

}
