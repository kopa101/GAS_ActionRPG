// Vince Petrelli All Rights Reserved


#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/WHeroWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WGameplayTags.h"

#include "WDebugHelper.h"

AWHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{   
    return Cast<AWHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

AWHeroWeapon* UHeroCombatComponent::GetHeroCurrentEquippedWeapon() const
{	
	return Cast<AWHeroWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UHeroCombatComponent::GetHeroCurrentEquippWeaponDamageAtLevel(float InLevel) const
{
	return GetHeroCurrentEquippedWeapon()->HeroWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}

void UHeroCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}
	 
	OverlappedActors.AddUnique(HitActor);

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		WGameplayTags::Shared_Event_MeleeHit,
		Data
	);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		WGameplayTags::Player_Event_HitPause,
		FGameplayEventData()
	);
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		WGameplayTags::Player_Event_HitPause,
		FGameplayEventData()
	);
}
