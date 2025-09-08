#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RNGService.generated.h"

UCLASS()
class URNGService : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Initialize with a base seed for the run
	UFUNCTION(BlueprintCallable)
	void InitializeRun(int32 BaseSeed);

	// Get or create a named stream derived from the base seed
	UFUNCTION(BlueprintCallable)
	FRandomStream& GetStream(FName StreamName);

	// Convenience random int in range using a named stream
	UFUNCTION(BlueprintCallable)
	int32 RandRange(FName StreamName, int32 Min, int32 Max);

private:
	int32 RunBaseSeed = 0;
	TMap<FName, FRandomStream> NameToStream;
};
