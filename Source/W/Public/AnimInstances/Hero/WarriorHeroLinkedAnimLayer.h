// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/WBaseAnimInstance.h"
#include "WHeroLinkedAnimLayer.generated.h"

class UWHeroAnimInstance;
/**
 * 
 */
UCLASS()
class W_API UWHeroLinkedAnimLayer : public UWBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UWHeroAnimInstance* GetHeroAnimInstance() const;
};
