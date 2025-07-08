// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WTypes/WEnumTypes.h"
#include "WSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class W_API UWSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	EWGameDifficulty SavedCurrentGameDifficulty;
};
