#include "GameModes/CGameMode.h"

ACGameMode::ACGameMode()
{
	ConstructorHelpers::FClassFinder<APawn> pawnClass(TEXT("/Script/Engine.Blueprint'/Game/_Project/Characters/BP_CPlayer.BP_CPlayer_C'"));
	if (pawnClass.Succeeded())
		DefaultPawnClass = pawnClass.Class;
}