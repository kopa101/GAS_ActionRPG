// Vince Petrelli All Rights Reserved


#include "WTypes/WCountDownAction.h"

void FWCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	if (bNeedToCancel)
	{
		CountDownOutput = EWCountDownActionOutput::Cancelled;

		Response.FinishAndTriggerIf(true,ExecutionFunction,OutputLink,CallbackTarget);

		return;
	}

	if (ElapsedTimeSinceStart >= TotalCountDownTime)
	{
		CountDownOutput = EWCountDownActionOutput::Completed;

		Response.FinishAndTriggerIf(true,ExecutionFunction,OutputLink,CallbackTarget);

		return;
	}

	if (ElapsedInterval < UpdateInterval)
	{
		ElapsedInterval += Response.ElapsedTime();
	}
	else
	{
		ElapsedTimeSinceStart += UpdateInterval > 0.f? UpdateInterval : Response.ElapsedTime();

		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;

		CountDownOutput = EWCountDownActionOutput::Updated;

		Response.TriggerLink(ExecutionFunction,OutputLink,CallbackTarget);
		  
		ElapsedInterval = 0.f;
	}
}

void FWCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}
