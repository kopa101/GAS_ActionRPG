// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WTypes/WEnumTypes.h"
#include "WFunctionLibrary.generated.h"

class UWAbilitySystemComponent;
class UPawnCombatComponent;
struct FScalableFloat;
class UWGameInstance;

/**
 * 
 */
UCLASS()
class W_API UWFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UWAbilitySystemComponent* NativeGetWASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "W|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor,FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "W|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor,FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor,FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "W|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor,FGameplayTag TagToCheck,EWConfirmType& OutConfirmType);

	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "W|FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor,EWValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "W|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn,APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "W|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat,float InLevel = 1.f);

	UFUNCTION(BlueprintPure, Category = "W|FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker,AActor* InVictim,float& OutAngleDifference);

	UFUNCTION(BlueprintPure, Category = "W|FunctionLibrary")
	static bool IsValidBlock(AActor* InAttacker,AActor* InDefender);

	UFUNCTION(BlueprintCallable,Category = "W|FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator,AActor* InTargetActor,const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "W|FunctionLibrary", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput",TotalTime = "1.0",UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject,float TotalTime,float UpdateInterval,
	float& OutRemainingTime,EWCountDownActionInput CountDownInput,
	UPARAM(DisplayName = "Output") EWCountDownActionOutput & CountDownOutput, FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintPure, Category = "W|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static UWGameInstance* GetWGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable,Category = "W|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static void ToggleInputMode(const UObject* WorldContextObject,EWInputMode InInputMode);

	UFUNCTION(BlueprintCallable,Category = "W|FunctionLibrary")
	static void SaveCurrentGameDifficulty(EWGameDifficulty InDifficultyToSave);

	UFUNCTION(BlueprintCallable,Category = "W|FunctionLibrary")
	static bool TryLoadSavedGameDifficulty(EWGameDifficulty& OutSavedDifficulty);
};
