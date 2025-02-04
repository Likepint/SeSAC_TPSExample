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

private:
	void InitializeCharacter();

};
