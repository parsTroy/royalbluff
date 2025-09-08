#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Types/CardTypes.h"
#include "ScoringSystem.generated.h"

USTRUCT(BlueprintType)
struct FScoreBreakdown
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 BaseChips = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BaseMult = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 BonusChips = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BonusMult = 0.0f;

	// Note: USTRUCTs cannot expose UFUNCTIONs. Provide a simple inline helper instead.
	int32 GetFinalScore() const { return FMath::FloorToInt((BaseChips + BonusChips) * (BaseMult + BonusMult)); }
};

UCLASS()
class UScoringSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FScoreBreakdown Score(const FHandEvaluationResult& Eval) const;
};
