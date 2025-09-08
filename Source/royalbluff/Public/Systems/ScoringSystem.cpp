#include "Systems/ScoringSystem.h"

static void GetBaselineForHand(EHandRank Hand, int32& OutChips, float& OutMult)
{
	switch (Hand)
	{
		case EHandRank::HighCard: OutChips = 30; OutMult = 1.0f; break;
		case EHandRank::OnePair: OutChips = 60; OutMult = 1.1f; break;
		case EHandRank::TwoPair: OutChips = 100; OutMult = 1.2f; break;
		case EHandRank::ThreeOfAKind: OutChips = 150; OutMult = 1.3f; break;
		case EHandRank::Straight: OutChips = 220; OutMult = 1.4f; break;
		case EHandRank::Flush: OutChips = 220; OutMult = 1.4f; break;
		case EHandRank::FullHouse: OutChips = 300; OutMult = 1.6f; break;
		case EHandRank::FourOfAKind: OutChips = 500; OutMult = 2.0f; break;
		case EHandRank::StraightFlush: OutChips = 800; OutMult = 3.0f; break;
		case EHandRank::RoyalFlush: OutChips = 1000; OutMult = 4.0f; break;
		default: OutChips = 0; OutMult = 1.0f; break;
	}
}

FScoreBreakdown UScoringSystem::Score(const FHandEvaluationResult& Eval) const
{
	FScoreBreakdown R;
	GetBaselineForHand(Eval.HandRank, R.BaseChips, R.BaseMult);
	// Placeholder: simple bonuses
	if (Eval.bUsedBothPocketCards)
	{
		R.BonusChips += 40;
	}
	if (Eval.bIsFlush && Eval.HandRank == EHandRank::Flush)
	{
		R.BonusMult += 0.1f;
	}
	return R;
}
