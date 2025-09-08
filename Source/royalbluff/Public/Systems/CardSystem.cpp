#include "Systems/CardSystem.h"
#include "Systems/RNGService.h"

void UCardSystem::InitializeStandardDeck()
{
	ResetAll();
	DrawPile.Reserve(52);
	for (uint8 Suit = (uint8)ECardSuit::Clubs; Suit <= (uint8)ECardSuit::Spades; ++Suit)
	{
		for (uint8 Rank = (uint8)ECardRank::Two; Rank <= (uint8)ECardRank::Ace; ++Rank)
		{
			FCard Card;
			Card.Suit = static_cast<ECardSuit>(Suit);
			Card.Rank = static_cast<ECardRank>(Rank);
			AssignUniqueId(Card);
			DrawPile.Add(Card);
		}
	}
}

void UCardSystem::ShuffleDeck(URNGService* RNGService, FName StreamName)
{
	if (!RNGService)
	{
		DrawPile.StableSort([](const FCard& A, const FCard& B){ return A.UniqueId < B.UniqueId; });
		return;
	}
	FRandomStream& Stream = RNGService->GetStream(StreamName);
	for (int32 i = DrawPile.Num() - 1; i > 0; --i)
	{
		int32 j = Stream.RandRange(0, i);
		if (i != j)
		{
			DrawPile.Swap(i, j);
		}
	}
}

bool TryPop(TArray<FCard>& From, FCard& Out)
{
	if (From.Num() == 0) return false;
	Out = From.Last();
	From.Pop(EAllowShrinking::No);
	return true;
}

FCard UCardSystem::DrawOne()
{
	FCard Card;
	if (!TryPop(DrawPile, Card))
	{
		// If draw is empty, recycle discard into draw (no shuffle here; caller can shuffle)
		DrawPile = MoveTemp(DiscardPile);
		DiscardPile.Reset();
		TryPop(DrawPile, Card);
	}
	return Card;
}

TArray<FCard> UCardSystem::Draw(int32 Count)
{
	TArray<FCard> Out;
	Out.Reserve(Count);
	for (int32 i = 0; i < Count; ++i)
	{
		Out.Add(DrawOne());
	}
	return Out;
}

void UCardSystem::Discard(const TArray<FCard>& Cards)
{
	DiscardPile.Append(Cards);
}

void UCardSystem::ResetAll()
{
	DrawPile.Reset();
	DiscardPile.Reset();
	NextUniqueId = 1;
}

void UCardSystem::AssignUniqueId(FCard& Card)
{
	Card.UniqueId = NextUniqueId++;
}
