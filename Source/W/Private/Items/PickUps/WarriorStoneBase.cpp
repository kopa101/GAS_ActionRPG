// Vince Petrelli All Rights Reserved


#include "Items/PickUps/WStoneBase.h"
#include "Characters/WHeroCharacter.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "WGameplayTags.h"

void AWStoneBase::Consume(UWAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{	
	check(StoneGameplayEffectClass);

	UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	
	AbilitySystemComponent->ApplyGameplayEffectToSelf(
		EffectCDO,
		ApplyLevel,
		AbilitySystemComponent->MakeEffectContext()
	);

	BP_OnStoneConsumed();
}

void AWStoneBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AWHeroCharacter* OverlappedHeroCharacter = Cast<AWHeroCharacter>(OtherActor))
	{
		OverlappedHeroCharacter->GetWAbilitySystemComponent()->TryActivateAbilityByTag(WGameplayTags::Player_Ability_PickUp_Stones);
	}
}
