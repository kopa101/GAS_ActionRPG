// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/WPickUpBase.h"
#include "WStoneBase.generated.h"

class UWAbilitySystemComponent;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class W_API AWStoneBase : public AWPickUpBase
{
	GENERATED_BODY()

public:
	void Consume(UWAbilitySystemComponent* AbilitySystemComponent,int32 ApplyLevel);

protected:
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Stone Consumed"))
	void BP_OnStoneConsumed();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;
};
