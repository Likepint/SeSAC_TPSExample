#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CTPSCharacter.generated.h"

UCLASS()
class TPSEXAMPLE_API ACTPSCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* Camera;

private:
	UPROPERTY(VisibleAnywhere, Category = "GunMesh")
	class USkeletalMeshComponent* RifleMesh;

	UPROPERTY(VisibleAnywhere, Category = "GunMesh")
	class UStaticMeshComponent* SniperMesh;

	UPROPERTY(EditAnywhere, Category = "BulletFactory")
	TSubclassOf<class ACBullet> BulletFactory;

private:
	// 스나이퍼 UI 위젯 팩토리
	UPROPERTY(EditDefaultsOnly, Category = "SniperUI")
	TSubclassOf<class UUserWidget> SniperUIFactory;

	// 스나이퍼 UI 위젯 인스턴스
	UPROPERTY()
	class UUserWidget* SniperUI;

	// 일반 조준 크로스헤어UI 위젯 팩토리
	UPROPERTY(EditDefaultsOnly, Category = "SniperUI")
	TSubclassOf<class UUserWidget> CrosshairUIFactory;

	// 일반 조준 크로스헤어UI 위젯 인스턴스
	UPROPERTY()
	class UUserWidget* CrossHairUI;

private:
	UPROPERTY ( EditAnywhere , Category = "BulletEffect" )
	class UParticleSystem* BulletEffectFactory;

private: // Input
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_TPS;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_VerticalLook;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_HorizontalLook;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Movement;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Rifle;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Sniper;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Fire;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Zoom;

public:
	ACTPSCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnVerticalLook(const struct FInputActionValue& InVal);
	void OnHorizontalLook(const struct FInputActionValue& InVal);

	void OnMovement(const struct FInputActionValue& InVal);

	void OnJumpAction(const struct FInputActionValue& InVal);

	void OnRifle(const struct FInputActionValue& InVal);
	void OnSniper(const struct FInputActionValue& InVal);

	void OnFire(const struct FInputActionValue& InVal);

	void OnZoom(const struct FInputActionValue& InVal);

private:
	void InitializeCharacter();

private:
	// 총의 종류를 구분하는 Bool변수
	bool bRifle = true;

	// 줌 상태인지를 구분하는 Bool변수
	bool bOnZoom = false;

};
