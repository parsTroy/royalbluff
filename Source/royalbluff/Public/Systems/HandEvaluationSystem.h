#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Types/CardTypes.h"
#include "HandEvaluationSystem.generated.h"

UCLASS()
class UHandEvaluationSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Evaluate the best scoring hand from up to 7 cards. Output includes metadata for scoring.
	UFUNCTION(BlueprintCallable)
	FHandEvaluationResult EvaluateBest5From7(const TArray<FCard>& Candidates) const;
};
