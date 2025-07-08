// Vince Petrelli All Rights Reserved


#include "Characters/WBaseCharacter.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "AbilitySystem/WAttributeSet.h"
#include "MotionWarpingComponent.h"

// Sets default values
AWBaseCharacter::AWBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	WAbilitySystemComponent = CreateDefaultSubobject<UWAbilitySystemComponent>(TEXT("WAbilitySystemComponent"));

	WAttributeSet = CreateDefaultSubobject<UWAttributeSet>(TEXT("WAttributeSet"));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UAbilitySystemComponent* AWBaseCharacter::GetAbilitySystemComponent() const
{
	return GetWAbilitySystemComponent();
}

UPawnCombatComponent* AWBaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* AWBaseCharacter::GetPawnUIComponent() const
{
	return nullptr;
}

void AWBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (WAbilitySystemComponent)
	{ 
		WAbilitySystemComponent->InitAbilityActorInfo(this,this);

		ensureMsgf(!CharacterStartUpData.IsNull(),TEXT("Forgot to assign start up data to %s"),*GetName());
	}
}

