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

public:
	ACTPSCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void InitializeCharacter();

};
