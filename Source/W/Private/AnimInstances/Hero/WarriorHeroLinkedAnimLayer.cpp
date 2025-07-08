// Vince Petrelli All Rights Reserved


#include "AnimInstances/Hero/WHeroLinkedAnimLayer.h"
#include "AnimInstances/Hero/WHeroAnimInstance.h"

UWHeroAnimInstance* UWHeroLinkedAnimLayer::GetHeroAnimInstance() const
{   
    return Cast<UWHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
