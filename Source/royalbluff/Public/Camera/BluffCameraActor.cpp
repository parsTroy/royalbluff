#include "Camera/BluffCameraActor.h"
#include "Camera/CameraComponent.h"

ABluffCameraActor::ABluffCameraActor()
{
	// Set up camera for UI viewing
	GetCameraComponent()->SetFieldOfView(90.0f);
	GetCameraComponent()->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	GetCameraComponent()->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
}
