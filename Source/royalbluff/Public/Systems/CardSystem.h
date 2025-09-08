#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Types/CardTypes.h"
#include "CardSystem.generated.h"

class URNGService;

UCLASS()
class UCardSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeStandardDeck();

	UFUNCTION(BlueprintCallable)
	void ShuffleDeck(URNGService* RNGService, FName StreamName = TEXT("Draws"));

	UFUNCTION(BlueprintCallable)
	bool CanDraw(int32 Count) const { return DrawPile.Num() >= Count; }

	UFUNCTION(BlueprintCallable)
	FCard DrawOne();

	UFUNCTION(BlueprintCallable)
	TArray<FCard> Draw(int32 Count);

	UFUNCTION(BlueprintCallable)
	void Discard(const TArray<FCard>& Cards);

	UFUNCTION(BlueprintCallable)
	int32 NumInDrawPile() const { return DrawPile.Num(); }

	UFUNCTION(BlueprintCallable)
	int32 NumInDiscardPile() const { return DiscardPile.Num(); }

	UFUNCTION(BlueprintCallable)
	void ResetAll();

private:
	void AssignUniqueId(FCard& Card);

private:
	TArray<FCard> DrawPile;
	TArray<FCard> DiscardPile;
	int32 NextUniqueId = 1;
};
