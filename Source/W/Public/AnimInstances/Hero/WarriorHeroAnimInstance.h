// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/WCharacterAnimInstance.h"
#include "WHeroAnimInstance.generated.h"

class AWHeroCharacter;

/**
 * 
 */
UCLASS()
class W_API UWHeroAnimInstance : public UWCharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Refrences")
	AWHeroCharacter* OwningHeroCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxtStateThreshold = 5.f;

	float IdleElpasedTime;
};
