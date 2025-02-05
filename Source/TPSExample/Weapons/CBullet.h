#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBullet.generated.h"

UCLASS()
class TPSEXAMPLE_API ACBullet : public AActor
{
	GENERATED_BODY()

private:
	// 충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Component")
	class USphereComponent* SphereCollision;

	// 외관 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Component")
	class UStaticMeshComponent* StaticMesh;

	// 발사체의 이동을 담당할 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* Projectile;
	
public:	
	ACBullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void Die();

};
