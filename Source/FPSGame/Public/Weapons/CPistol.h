#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPistol.generated.h"

UCLASS()
class FPSGAME_API ACPistol : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPistol();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	static ACPistol* Spawn(class UWorld* InWorld, class ACharacter* InOwnerCharacter);
	void Begin_Fire();
	void End_Fire();
	void Firing();

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class USkeletalMeshComponent* Mesh;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
		TSubclassOf<class ACBullet> BulletClass;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
		float PitchSpeed = 0.25f;

	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
		TSubclassOf<class UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
		class UParticleSystem* ImpactParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
		class UMaterialInstanceConstant* DecalMaterial;

private:
	class ACharacter* OwnerCharacter;

	float CurrentPitch;

	bool bFiring;
};
