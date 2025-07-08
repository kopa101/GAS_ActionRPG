#pragma once

UENUM()
enum class EWConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EWValidType : uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EWSuccessType : uint8
{
	Successful,
	Failed
};

UENUM()
enum class EWCountDownActionInput : uint8
{
	Start,
	Cancel
};

UENUM()
enum class EWCountDownActionOutput : uint8
{
	Updated,
	Completed,
	Cancelled
};

UENUM(BlueprintType)
enum class EWGameDifficulty : uint8
{
	Easy,
	Normal,
	Hard,
	VeryHard
};

UENUM(BlueprintType)
enum class EWInputMode : uint8
{
	GameOnly,
	UIOnly
};
