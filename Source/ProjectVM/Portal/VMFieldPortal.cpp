
#include "Portal/VMFieldPortal.h"

AVMFieldPortal::AVMFieldPortal()
{
	PlayerTeleportPos = FVector(-4000.f, -900.f, 500.f);
}

void AVMFieldPortal::Interact()
{
	Super::Interact();

	TeleportPlayerToMap();
}
