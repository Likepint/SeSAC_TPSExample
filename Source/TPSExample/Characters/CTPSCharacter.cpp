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
	// Mesh�� SK_Mannequin �ε� �� ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(L"/Script/Engine.SkeletalMesh'/Game/PJS/Characters/Meshes/SK_Mannequin.SK_Mannequin'");
	if (mesh.Succeeded()) // �����̶��
		GetMesh()->SetSkeletalMesh(mesh.Object);

	// ��ġ���� ȸ���� �ݿ�
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));

	// SpringArm Component ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	// Root Component�� Attach
	SpringArm->SetupAttachment(RootComponent);
	// �����ġ ����
	SpringArm->SetRelativeLocation(FVector(0, 60, 80));
	// TargetArmLength ����
	SpringArm->TargetArmLength = 300;

	// Camera Component ����
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	// SpringArm Component�� Attach
	Camera->SetupAttachment(SpringArm);

}
