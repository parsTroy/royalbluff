#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BluffHUDWidget.generated.h"

class UTextBlock;

UCLASS()
class UBluffHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetBoardText(const FString& InText);

	UFUNCTION(BlueprintCallable)
	void SetCountersText(const FString& InText);

	UFUNCTION(BlueprintCallable)
	void SetScoreText(const FString& InText);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	UTextBlock* BoardText = nullptr;

	UPROPERTY()
	UTextBlock* CountersText = nullptr;

	UPROPERTY()
	UTextBlock* ScoreText = nullptr;
};
