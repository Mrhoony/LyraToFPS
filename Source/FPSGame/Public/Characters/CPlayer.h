#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/IWeapon.h"
#include "CPlayer.generated.h"

UCLASS()
class FPSGAME_API ACPlayer : public ACharacter, public IIWeapon
{
	GENERATED_BODY()

public:
	ACPlayer();

	//FORCEINLINE class AActor* GetWeapon() override { return CurWeapon; }
	FORCEINLINE class AActor* GetWeapon() override { return CurWeapon; }
	void GetAimInfo(FVector& OutAimStart, FVector& OutAimEnd, FVector& OutAimDirection) override;
	void OnTarget() override;
	void OffTarget() override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);

	void OnFire();
	void OffFire();

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

private:
	class AActor* CurWeapon;
	class ACPistol* Pistol;
};
