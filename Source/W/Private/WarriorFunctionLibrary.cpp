// Vince Petrelli All Rights Reserved

#include "WFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "WGameplayTags.h"
#include "WTypes/WCountDownAction.h"
#include "WGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/WSaveGame.h"

#include "WDebugHelper.h"

UWAbilitySystemComponent* UWFunctionLibrary::NativeGetWASCFromActor(AActor* InActor)
{   
    check(InActor);

    return CastChecked<UWAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UWFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
    UWAbilitySystemComponent* ASC = NativeGetWASCFromActor(InActor);

    if (!ASC->HasMatchingGameplayTag(TagToAdd))
    {
        ASC->AddLooseGameplayTag(TagToAdd);
    }
}

void UWFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
    UWAbilitySystemComponent* ASC = NativeGetWASCFromActor(InActor);

    if (ASC->HasMatchingGameplayTag(TagToRemove))
    {
        ASC->RemoveLooseGameplayTag(TagToRemove);
    }
}

bool UWFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
    UWAbilitySystemComponent* ASC = NativeGetWASCFromActor(InActor);

    return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UWFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWConfirmType& OutConfirmType)
{
    OutConfirmType = NativeDoesActorHaveTag(InActor,TagToCheck)? EWConfirmType::Yes : EWConfirmType::No;
}

UPawnCombatComponent* UWFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{   
    check(InActor);

    if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
    {
        return PawnCombatInterface->GetPawnCombatComponent();
    }

    return nullptr;
}

UPawnCombatComponent* UWFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EWValidType& OutValidType)
{   
    UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

    OutValidType = CombatComponent? EWValidType::Valid : EWValidType::Invalid;

    return CombatComponent;
}

bool UWFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{   
    check(QueryPawn && TargetPawn);

    IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
    IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

    if (QueryTeamAgent && TargetTeamAgent)
    {
        return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
    }

    return false;
}

float UWFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
    return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UWFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference)
{   
    check(InAttacker && InVictim);

    const FVector VictimForward = InVictim->GetActorForwardVector();
    const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

    const float DotResult = FVector::DotProduct(VictimForward,VictimToAttackerNormalized);
    OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

    const FVector CrossResult = FVector::CrossProduct(VictimForward,VictimToAttackerNormalized);

    if (CrossResult.Z < 0.f)
    {
        OutAngleDifference *= -1.f;
    }

    if (OutAngleDifference>=-45.f && OutAngleDifference <=45.f)
    {
        return WGameplayTags::Shared_Status_HitReact_Front;
    }
    else if (OutAngleDifference<-45.f && OutAngleDifference>=-135.f)
    {
        return WGameplayTags::Shared_Status_HitReact_Left;
    }
    else if (OutAngleDifference<-135.f || OutAngleDifference>135.f)
    {
        return WGameplayTags::Shared_Status_HitReact_Back;
    }
    else if(OutAngleDifference>45.f && OutAngleDifference<=135.f)
    {
        return WGameplayTags::Shared_Status_HitReact_Right;
    }

    return WGameplayTags::Shared_Status_HitReact_Front;
}

bool UWFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{   
    check(InAttacker && InDefender);

    const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(),InDefender->GetActorForwardVector());

   /* const FString DebugString = FString::Printf(TEXT("Dot Result: %f %s"),DotResult,DotResult<-0.1f? TEXT("Valid Block") : TEXT("InvalidBlock"));

    Debug::Print(DebugString,DotResult<-0.1f? FColor::Green : FColor::Red);*/

    return DotResult<-0.1f;
}

bool UWFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{   
    UWAbilitySystemComponent* SourceASC = NativeGetWASCFromActor(InInstigator);
    UWAbilitySystemComponent* TargetASC = NativeGetWASCFromActor(InTargetActor);

    FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data,TargetASC);

    return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

void UWFunctionLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, EWCountDownActionInput CountDownInput, UPARAM(DisplayName = "Output") EWCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo)
{
    UWorld* World = nullptr;

    if (GEngine)
    {
        World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull);
    }

    if (!World)
    {
        return;
    }

    FLatentActionManager& LatentActionManager = World->GetLatentActionManager();

    FWCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FWCountDownAction>(LatentInfo.CallbackTarget,LatentInfo.UUID);

    if (CountDownInput == EWCountDownActionInput::Start)
    {
        if (!FoundAction)
        {
            LatentActionManager.AddNewAction(
                LatentInfo.CallbackTarget,
                LatentInfo.UUID,
                new FWCountDownAction(TotalTime,UpdateInterval,OutRemainingTime,CountDownOutput,LatentInfo)
            );
        }
    }

    if (CountDownInput == EWCountDownActionInput::Cancel)
    {
        if (FoundAction)
        {
            FoundAction->CancelAction();
        }
    }
    
}

UWGameInstance* UWFunctionLibrary::GetWGameInstance(const UObject* WorldContextObject)
{   
    if (GEngine)
    {
        if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
        {
            return World->GetGameInstance<UWGameInstance>();
        }
    }

    return nullptr;
}

void UWFunctionLibrary::ToggleInputMode(const UObject* WorldContextObject, EWInputMode InInputMode)
{
	APlayerController* PlayerController = nullptr;

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			PlayerController = World->GetFirstPlayerController();
		}
	}

	if (!PlayerController)
	{
		return;
	}

    FInputModeGameOnly GameOnlyMode;
    FInputModeUIOnly UIOnlyMode;

    switch (InInputMode)
    {
    case EWInputMode::GameOnly:
        
        PlayerController->SetInputMode(GameOnlyMode);
        PlayerController->bShowMouseCursor = false;

        break;

    case EWInputMode::UIOnly:

		PlayerController->SetInputMode(UIOnlyMode);
		PlayerController->bShowMouseCursor = true;

        break;

    default:
        break;
    }
}

void UWFunctionLibrary::SaveCurrentGameDifficulty(EWGameDifficulty InDifficultyToSave)
{
    USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(UWSaveGame::StaticClass());

    if (UWSaveGame* WSaveGameObject = Cast<UWSaveGame>(SaveGameObject))
    {
        WSaveGameObject->SavedCurrentGameDifficulty = InDifficultyToSave;

        const bool bWasSaved = UGameplayStatics::SaveGameToSlot(WSaveGameObject,WGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(),0);

        Debug::Print(bWasSaved? TEXT("Difficulty Saved") : TEXT("Difficulty NOT Saved"));
    }
}

bool UWFunctionLibrary::TryLoadSavedGameDifficulty(EWGameDifficulty& OutSavedDifficulty)
{
    if (UGameplayStatics::DoesSaveGameExist(WGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0))
    {
        USaveGame* SaveGameObject = UGameplayStatics::LoadGameFromSlot(WGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(),0);

		if (UWSaveGame* WSaveGameObject = Cast<UWSaveGame>(SaveGameObject))
		{
            OutSavedDifficulty = WSaveGameObject->SavedCurrentGameDifficulty;

            Debug::Print(TEXT("Loading Successful"),FColor::Green);

            return true;
		}
    }

    return false;
}
