#include "Characters/CTPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ACTPSCharacter::ACTPSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	InitializeCharacter();

}

void ACTPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACTPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACTPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACTPSCharacter::InitializeCharacter()
{
	// Mesh에 SK_Mannequin 로드 후 설정
	ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(L"/Script/Engine.SkeletalMesh'/Game/PJS/Characters/Meshes/SK_Mannequin.SK_Mannequin'");
	if (mesh.Succeeded()) // 성공이라면
		GetMesh()->SetSkeletalMesh(mesh.Object);

	// 위치값과 회전값 반영
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));

	// SpringArm Component 생성
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	// Root Component에 Attach
	SpringArm->SetupAttachment(RootComponent);
	// 상대위치 지정
	SpringArm->SetRelativeLocation(FVector(0, 60, 80));
	// TargetArmLength 지정
	SpringArm->TargetArmLength = 300;

	// Camera Component 생성
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	// SpringArm Component에 Attach
	Camera->SetupAttachment(SpringArm);

}
