// Vince Petrelli All Rights Reserved


#include "AbilitySystem/Abilities/WEnemyGameplayAbility.h"
#include "Characters/WEnemyCharacter.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "WGameplayTags.h"

AWEnemyCharacter* UWEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{   
	if (!CachedWEnemyCharacter.IsValid())
	{
		CachedWEnemyCharacter = Cast<AWEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}

    return CachedWEnemyCharacter.IsValid()? CachedWEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UWEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
    return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}

FGameplayEffectSpecHandle UWEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
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
		InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
	);

	return EffectSpecHandle;
}
