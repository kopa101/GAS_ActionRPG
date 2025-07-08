// Vince Petrelli All Rights Reserved


#include "AbilitySystem/Abilities/WHeroGameplayAbility.h"
#include "Characters/WHeroCharacter.h"
#include "Controllers/WHeroController.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "WGameplayTags.h"

AWHeroCharacter* UWHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{   
	if (!CachedWHeroCharacter.IsValid())
	{
		CachedWHeroCharacter = Cast<AWHeroCharacter>(CurrentActorInfo->AvatarActor);
	}
   
    return CachedWHeroCharacter.IsValid()? CachedWHeroCharacter.Get() : nullptr;
}

AWHeroController* UWHeroGameplayAbility::GetHeroControllerFromActorInfo()
{	
	if (!CachedWHeroController.IsValid())
	{
		CachedWHeroController = Cast<AWHeroController>(CurrentActorInfo->PlayerController);
	}

	return CachedWHeroController.IsValid()? CachedWHeroController.Get() : nullptr;
}

UHeroCombatComponent* UWHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}

UHeroUIComponent* UWHeroGameplayAbility::GetHeroUIComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetHeroUIComponent();
}

FGameplayEffectSpecHandle UWHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
{	
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetWAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(),GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetWAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		WGameplayTags::Shared_SetByCaller_BaseDamage,
		InWeaponBaseDamage
	);

	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag,InUsedComboCount);
	}

	return EffectSpecHandle;
}

bool UWHeroGameplayAbility::GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime)
{
	check(InCooldownTag.IsValid());

	FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());

	TArray< TPair <float,float> > TimeRemainingAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

	if (!TimeRemainingAndDuration.IsEmpty())
	{
		RemainingCooldownTime = TimeRemainingAndDuration[0].Key;
		TotalCooldownTime = TimeRemainingAndDuration[0].Value;
	}

	return RemainingCooldownTime > 0.f;
}
