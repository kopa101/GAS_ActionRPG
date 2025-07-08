// Vince Petrelli All Rights Reserved


#include "Characters/WHeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/WInputComponent.h"
#include "WGameplayTags.h"
#include "AbilitySystem/WAbilitySystemComponent.h"
#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/UI/HeroUIComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameModes/WBaseGameMode.h"

#include "WDebugHelper.h"

AWHeroCharacter::AWHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f,96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f,55.f,65.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,500.f,0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));

	HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("HeroUIComponent"));
}

UPawnCombatComponent* AWHeroCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponent;
}

UPawnUIComponent* AWHeroCharacter::GetPawnUIComponent() const
{
	return HeroUIComponent;
}

UHeroUIComponent* AWHeroCharacter::GetHeroUIComponent() const
{
	return HeroUIComponent;
}

void AWHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{	
			int32 AbilityApplyLevel = 1;

			if (AWBaseGameMode* BaseGameMode = GetWorld()->GetAuthGameMode<AWBaseGameMode>())
			{
				switch (BaseGameMode->GetCurrentGameDifficulty())
				{
				case EWGameDifficulty::Easy:
					AbilityApplyLevel = 4;
					Debug::Print(TEXT("Current Difficulty: Easy"));
					break;

				case EWGameDifficulty::Normal:
					AbilityApplyLevel = 3;
					Debug::Print(TEXT("Current Difficulty: Normal"));
					break;

				case EWGameDifficulty::Hard:
					AbilityApplyLevel = 2;
					Debug::Print(TEXT("Current Difficulty: Hard"));
					break;

				case EWGameDifficulty::VeryHard:
					AbilityApplyLevel = 1;
					Debug::Print(TEXT("Current Difficulty: Very Hard"));
					break;

				default:
					break;
				}
			}
			
			LoadedData->GiveToAbilitySystemComponent(WAbilitySystemComponent,AbilityApplyLevel);
		}
	}
}

void AWHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	checkf(InputConfigDataAsset,TEXT("Forgot to assign a valid data asset as input config"));

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext,0);

	UWInputComponent* WInputComponent = CastChecked<UWInputComponent>(PlayerInputComponent);

	WInputComponent->BindNativeInputAction(InputConfigDataAsset,WGameplayTags::InputTag_Move,ETriggerEvent::Triggered,this,&ThisClass::Input_Move);
	WInputComponent->BindNativeInputAction(InputConfigDataAsset,WGameplayTags::InputTag_Look,ETriggerEvent::Triggered,this,&ThisClass::Input_Look);

	WInputComponent->BindNativeInputAction(InputConfigDataAsset,WGameplayTags::InputTag_SwitchTarget,ETriggerEvent::Triggered,this,&ThisClass::Input_SwitchTargetTriggered);
	WInputComponent->BindNativeInputAction(InputConfigDataAsset,WGameplayTags::InputTag_SwitchTarget,ETriggerEvent::Completed,this,&ThisClass::Input_SwitchTargetCompleted);

	WInputComponent->BindNativeInputAction(InputConfigDataAsset,WGameplayTags::InputTag_PickUp_Stones,ETriggerEvent::Started,this,&ThisClass::Input_PickUpStonesStarted);

	WInputComponent->BindAbilityInputAction(InputConfigDataAsset,this,&ThisClass::Input_AbilityInputPressed,&ThisClass::Input_AbilityInputReleased);
}

void AWHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AWHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{	
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	const FRotator MovementRotation(0.f,Controller->GetControlRotation().Yaw,0.f);

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection,MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection,MovementVector.X);
	}
}

void AWHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	
	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AWHeroCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void AWHeroCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{	
	FGameplayEventData Data;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		SwitchDirection.X>0.f? WGameplayTags::Player_Event_SwitchTarget_Right : WGameplayTags::Player_Event_SwitchTarget_Left,
		Data
	);
}

void AWHeroCharacter::Input_PickUpStonesStarted(const FInputActionValue& InputActionValue)
{	
	FGameplayEventData Data;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		WGameplayTags::Player_Event_ConsumeStones,
		Data
	);
}

void AWHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	WAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AWHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	WAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}
