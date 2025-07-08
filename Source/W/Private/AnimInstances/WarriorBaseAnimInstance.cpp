// Vince Petrelli All Rights Reserved


#include "AnimInstances/WBaseAnimInstance.h"
#include "WFunctionLibrary.h"

bool UWBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UWFunctionLibrary::NativeDoesActorHaveTag(OwningPawn,TagToCheck);
	}

	return false;
}
