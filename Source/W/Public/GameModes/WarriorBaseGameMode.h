// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WTypes/WEnumTypes.h"
#include "WBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class W_API AWBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWBaseGameMode();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	EWGameDifficulty CurrentGameDifficulty;

public:
	FORCEINLINE EWGameDifficulty GetCurrentGameDifficulty() const { return CurrentGameDifficulty;}
};
