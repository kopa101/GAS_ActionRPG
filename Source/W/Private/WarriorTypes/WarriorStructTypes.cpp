// Vince Petrelli All Rights Reserved


#include "WTypes/WStructTypes.h"
#include "AbilitySystem/Abilities/WHeroGameplayAbility.h"

bool FWHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
