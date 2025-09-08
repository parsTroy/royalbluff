#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Types/CardTypes.h"
#include "BluffGameState.generated.h"

UCLASS()
class ABluffGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentAnte() const { return CurrentAnte; }

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentBlindIndex() const { return CurrentBlindIndex; }

	UFUNCTION(BlueprintCallable)
	int32 GetCurrency() const { return Currency; }

	UFUNCTION(BlueprintCallable)
	void AddCurrency(int32 Amount) { Currency = FMath::Max(0, Currency + Amount); }

	UFUNCTION(BlueprintCallable)
	void AdvanceBlind() { ++CurrentBlindIndex; }

	UFUNCTION(BlueprintCallable)
	void AdvanceAnte() { ++CurrentAnte; CurrentBlindIndex = 0; }

	// Board state
	UFUNCTION(BlueprintCallable)
	const TArray<FCard>& GetPocket() const { return PocketCards; }

	UFUNCTION(BlueprintCallable)
	const TArray<FCard>& GetCommunity() const { return CommunityCards; }

	UFUNCTION(BlueprintCallable)
	void SetPocket(const TArray<FCard>& InPocket) { PocketCards = InPocket; }

	UFUNCTION(BlueprintCallable)
	void ClearBoard() { PocketCards.Reset(); CommunityCards.Reset(); }

	UFUNCTION(BlueprintCallable)
	void AddCommunityCards(const TArray<FCard>& InCards) { CommunityCards.Append(InCards); }

	// Counters per blind
	UFUNCTION(BlueprintCallable)
	void SetPlaysAndDiscards(int32 InPlays, int32 InDiscards) { PlaysRemaining = InPlays; DiscardsRemaining = InDiscards; }

	UFUNCTION(BlueprintCallable)
	int32 GetPlaysRemaining() const { return PlaysRemaining; }

	UFUNCTION(BlueprintCallable)
	int32 GetDiscardsRemaining() const { return DiscardsRemaining; }

	UFUNCTION(BlueprintCallable)
	bool ConsumePlay() { if (PlaysRemaining <= 0) return false; --PlaysRemaining; return true; }

	UFUNCTION(BlueprintCallable)
	bool ConsumeDiscard() { if (DiscardsRemaining <= 0) return false; --DiscardsRemaining; return true; }

	// Last score
	UFUNCTION(BlueprintCallable)
	int32 GetLastFinalScore() const { return LastFinalScore; }
	UFUNCTION(BlueprintCallable)
	void SetLastFinalScore(int32 InScore) { LastFinalScore = InScore; }

private:
	UPROPERTY(VisibleAnywhere)
	int32 CurrentAnte = 1;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentBlindIndex = 0;

	UPROPERTY(VisibleAnywhere)
	int32 Currency = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<FCard> PocketCards;

	UPROPERTY(VisibleAnywhere)
	TArray<FCard> CommunityCards;

	UPROPERTY(VisibleAnywhere)
	int32 PlaysRemaining = 3;

	UPROPERTY(VisibleAnywhere)
	int32 DiscardsRemaining = 5;

	UPROPERTY(VisibleAnywhere)
	int32 LastFinalScore = 0;
};
