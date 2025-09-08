#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BluffGameModeBase.generated.h"

UCLASS()
class ABluffGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABluffGameModeBase();

protected:
	virtual void BeginPlay() override;

	// If 0, will auto-seed from time; else use provided seed
	UPROPERTY(EditAnywhere, Category = "Run")
	int32 DebugSeed = 0;

public:
	UFUNCTION(Exec)
	void RevealTurn();

	UFUNCTION(Exec)
	void RevealRiver();

	UFUNCTION(Exec)
	void ScoreCurrentHand();
};
