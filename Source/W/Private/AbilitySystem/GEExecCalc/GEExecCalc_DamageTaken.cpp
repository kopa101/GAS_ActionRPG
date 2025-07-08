// Vince Petrelli All Rights Reserved


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/WAttributeSet.h"
#include "WGameplayTags.h"

#include "WDebugHelper.h"

struct FWDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FWDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWAttributeSet,AttackPower,Source,false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWAttributeSet,DefensePower,Target,false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWAttributeSet,DamageTaken,Target,false)
	}
};

static const FWDamageCapture& GetWDamageCapture()
{
	static FWDamageCapture WDamageCapture;
	return WDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{	
	/*Slow way of doing capture*/
	//FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
	//	UWAttributeSet::StaticClass(),
	//	GET_MEMBER_NAME_CHECKED(UWAttributeSet,AttackPower)
	//);

	//FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
	//	AttackPowerProperty,
	//	EGameplayEffectAttributeCaptureSource::Source,
	//	false
	//);

	//RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);

	RelevantAttributesToCapture.Add(GetWDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetWDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetWDamageCapture().DamageTakenDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{	
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	
	/*EffectSpec.GetContext().GetSourceObject();
	EffectSpec.GetContext().GetAbility();
	EffectSpec.GetContext().GetInstigator();
	EffectSpec.GetContext().GetEffectCauser();*/

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWDamageCapture().AttackPowerDef,EvaluateParameters,SourceAttackPower);
	/*Debug::Print(TEXT("SourceAttackPower"),SourceAttackPower);*/

	float BaseDamage = 0.f;
	int32 UsedLightAttckComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;

	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(WGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
			/*Debug::Print(TEXT("BaseDamage"),BaseDamage);*/
		}

		if (TagMagnitude.Key.MatchesTagExact(WGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttckComboCount = TagMagnitude.Value;
			/*Debug::Print(TEXT("UsedLightAttckComboCount"),UsedLightAttckComboCount);*/
		}

		if (TagMagnitude.Key.MatchesTagExact(WGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
		/*	Debug::Print(TEXT("UsedHeavyAttackComboCount"),UsedHeavyAttackComboCount);*/
		}
	}
	
	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWDamageCapture().DefensePowerDef,EvaluateParameters,TargetDefensePower);
	/*Debug::Print(TEXT("TargetDefensePower"),TargetDefensePower);*/

	if (UsedLightAttckComboCount != 0)
	{
		const float DamageIncreasePercentLight = (UsedLightAttckComboCount - 1) * 0.05f + 1.f;

		BaseDamage *= DamageIncreasePercentLight;
		/*Debug::Print(TEXT("ScaledBaseDamageLight"),BaseDamage);*/
	}

	if (UsedHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = UsedHeavyAttackComboCount * 0.15f + 1.f;

		BaseDamage *= DamageIncreasePercentHeavy;
		/*Debug::Print(TEXT("ScaledBaseDamageHeavy"),BaseDamage);*/
	}

	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	/*Debug::Print(TEXT("FinalDamageDone"),FinalDamageDone);*/

	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetWDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}
