// Vince Petrelli All Rights Reserved


#include "Controllers/WHeroController.h"

AWHeroController::AWHeroController()
{
	HeroTeamID = FGenericTeamId(0);
}

FGenericTeamId AWHeroController::GetGenericTeamId() const
{
	return HeroTeamID;
}
