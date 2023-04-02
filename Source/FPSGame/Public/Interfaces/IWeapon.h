#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWeapon.generated.h"

UINTERFACE(MinimalAPI)
class UIWeapon : public UInterface
{
	GENERATED_BODY()
};

class FPSGAME_API IIWeapon
{
	GENERATED_BODY()

public:
	virtual class AActor* GetWeapon() = 0;
	virtual void GetAimInfo(FVector& OutAimStart, FVector& OutAimEnd, FVector& OutAimDirection) = 0;

	virtual void OnTarget() {};
	virtual void OffTarget() {};
};
