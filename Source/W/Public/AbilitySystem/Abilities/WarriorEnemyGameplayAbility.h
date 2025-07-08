// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WGameplayAbility.h"
#include "WEnemyGameplayAbility.generated.h"

class AWEnemyCharacter;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class W_API UWEnemyGameplayAbility : public UWGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "W|Ability")
	AWEnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "W|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "W|Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,const FScalableFloat& InDamageScalableFloat);

private:
	TWeakObjectPtr<AWEnemyCharacter> CachedWEnemyCharacter;
};
