// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/WBaseAnimInstance.h"
#include "WCharacterAnimInstance.generated.h"

class AWBaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class W_API UWCharacterAnimInstance : public UWBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	AWBaseCharacter* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;
};
