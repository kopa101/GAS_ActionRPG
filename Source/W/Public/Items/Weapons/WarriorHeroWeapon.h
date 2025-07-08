// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/WWeaponBase.h"
#include "WTypes/WStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "WHeroWeapon.generated.h"

/**
 * 
 */
UCLASS()
class W_API AWHeroWeapon : public AWWeaponBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FWHeroWeaponData HeroWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
 