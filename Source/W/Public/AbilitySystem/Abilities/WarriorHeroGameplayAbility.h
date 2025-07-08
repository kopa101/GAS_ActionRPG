// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WGameplayAbility.h"
#include "WHeroGameplayAbility.generated.h"

class AWHeroCharacter;
class AWHeroController;

/**
 * 
 */
UCLASS()
class W_API UWHeroGameplayAbility : public UWGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "W|Ability")
	AWHeroCharacter* GetHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "W|Ability")
	AWHeroController* GetHeroControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "W|Ability")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "W|Ability")
	UHeroUIComponent* GetHeroUIComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "W|Ability")
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,float InWeaponBaseDamage,FGameplayTag InCurrentAttackTypeTag,int32 InUsedComboCount);
	
	UFUNCTION(BlueprintCallable, Category = "W|Ability")
	bool GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag,float& TotalCooldownTime,float& RemainingCooldownTime);

private:
	TWeakObjectPtr<AWHeroCharacter> CachedWHeroCharacter;
	TWeakObjectPtr<AWHeroController> CachedWHeroController;
};
