#include "Systems/HandEvaluationSystem.h"

static void SortRanksDesc(TArray<ECardRank>& Ranks)
{
	Ranks.Sort([](ECardRank A, ECardRank B)
	{
		return (uint8)A > (uint8)B;
	});
}

FHandEvaluationResult UHandEvaluationSystem::EvaluateBest5From7(const TArray<FCard>& Candidates) const
{
	FHandEvaluationResult Result;
	// Placeholder: take top 5 by rank; mark as HighCard
	TArray<FCard> Sorted = Candidates;
	Sorted.Sort([](const FCard& A, const FCard& B){ return (uint8)A.Rank > (uint8)B.Rank; });
	const int32 Take = FMath::Min(5, Sorted.Num());
	for (int32 i = 0; i < Take; ++i)
	{
		Result.ScoringCards.Add(Sorted[i]);
		Result.RankOrder.Add(Sorted[i].Rank);
	}
	SortRanksDesc(Result.RankOrder);
	Result.HandRank = EHandRank::HighCard;
	Result.bIsFlush = false;
	Result.bIsStraight = false;
	Result.bUsedBothPocketCards = false; // to be set by calling context when known
	return Result;
}
