#pragma once

#include "CoreMinimal.h"
#include "CardTypes.generated.h"

UENUM(BlueprintType)
enum class ECardSuit : uint8
{
	Clubs		UMETA(DisplayName = "Clubs"),
	Diamonds	UMETA(DisplayName = "Diamonds"),
	Hearts		UMETA(DisplayName = "Hearts"),
	Spades		UMETA(DisplayName = "Spades")
};

UENUM(BlueprintType)
enum class ECardRank : uint8
{
	Two		UMETA(DisplayName = "2"),
	Three	UMETA(DisplayName = "3"),
	Four	UMETA(DisplayName = "4"),
	Five	UMETA(DisplayName = "5"),
	Six		UMETA(DisplayName = "6"),
	Seven	UMETA(DisplayName = "7"),
	Eight	UMETA(DisplayName = "8"),
	Nine	UMETA(DisplayName = "9"),
	Ten		UMETA(DisplayName = "10"),
	Jack	UMETA(DisplayName = "J"),
	Queen	UMETA(DisplayName = "Q"),
	King	UMETA(DisplayName = "K"),
	Ace		UMETA(DisplayName = "A")
};

UENUM(BlueprintType)
enum class EHandRank : uint8
{
	HighCard		UMETA(DisplayName = "High Card"),
	OnePair			UMETA(DisplayName = "One Pair"),
	TwoPair			UMETA(DisplayName = "Two Pair"),
	ThreeOfAKind	UMETA(DisplayName = "Three of a Kind"),
	Straight		UMETA(DisplayName = "Straight"),
	Flush			UMETA(DisplayName = "Flush"),
	FullHouse		UMETA(DisplayName = "Full House"),
	FourOfAKind		UMETA(DisplayName = "Four of a Kind"),
	StraightFlush	UMETA(DisplayName = "Straight Flush"),
	RoyalFlush		UMETA(DisplayName = "Royal Flush")
};

USTRUCT(BlueprintType)
struct FCard
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardRank Rank = ECardRank::Ace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardSuit Suit = ECardSuit::Spades;

	// Unique runtime id for tracking effects; 0 means unset
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 UniqueId = 0;
};

USTRUCT(BlueprintType)
struct FHandEvaluationResult
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EHandRank HandRank = EHandRank::HighCard;

	// Sorted primary ranks used for tie-breakers (e.g., quads rank, trips rank, pair ranks, kickers)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ECardRank> RankOrder;

	// The exact 5 cards used for the scoring hand (snapshot)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FCard> ScoringCards;

	// Metadata flags to feed scoring/modifiers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bUsedBothPocketCards = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFlush = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsStraight = false;
};
