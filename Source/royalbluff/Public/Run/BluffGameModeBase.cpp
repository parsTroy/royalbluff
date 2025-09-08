#include "Run/BluffGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Systems/RNGService.h"
#include "Systems/CardSystem.h"
#include "Systems/HandEvaluationSystem.h"
#include "Systems/ScoringSystem.h"
#include "UI/BluffHUDWidget.h"
#include "Run/BluffGameState.h"

ABluffGameModeBase::ABluffGameModeBase()
{
	// Set the GameState class
	GameStateClass = ABluffGameState::StaticClass();
	UE_LOG(LogTemp, Warning, TEXT("ABluffGameModeBase constructor called! GameStateClass set to: %s"), 
		GameStateClass ? *GameStateClass->GetName() : TEXT("NULL"));
}

static FString CardToString(const FCard& C)
{
	static const TCHAR* SuitStr[] = { TEXT("C"), TEXT("D"), TEXT("H"), TEXT("S") };
	static const TCHAR* RankStr[] = { TEXT("2"), TEXT("3"), TEXT("4"), TEXT("5"), TEXT("6"), TEXT("7"), TEXT("8"), TEXT("9"), TEXT("10"), TEXT("J"), TEXT("Q"), TEXT("K"), TEXT("A") };
	return FString::Printf(TEXT("%s%s"), RankStr[(int32)C.Rank], SuitStr[(int32)C.Suit]);
}

void ABluffGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay called! GameStateClass: %s"), 
		GameStateClass ? *GameStateClass->GetName() : TEXT("NULL"));
	
	ABluffGameState* GameState = GetGameState<ABluffGameState>();
	UE_LOG(LogTemp, Warning, TEXT("GetGameState result: %s"), GameState ? TEXT("Found") : TEXT("NULL"));

	int32 SeedToUse = DebugSeed;
	if (SeedToUse == 0)
	{
		SeedToUse = FDateTime::UtcNow().GetMillisecond() ^ FDateTime::UtcNow().GetSecond() ^ FPlatformTime::Cycles();
	}

	URNGService* RNG = nullptr;
	UCardSystem* Cards = nullptr;

	if (UGameInstance* GI = GetGameInstance())
	{
		// Ensure subsystems are created
		RNG = GI->GetSubsystem<URNGService>();
		Cards = GI->GetSubsystem<UCardSystem>();
		
		// Force creation of subsystems if they don't exist
		if (!RNG)
		{
			RNG = GI->GetSubsystem<URNGService>();
		}
		if (!Cards)
		{
			Cards = GI->GetSubsystem<UCardSystem>();
		}
		
		if (RNG)
		{
			RNG->InitializeRun(SeedToUse);
		}
		if (Cards)
		{
			Cards->InitializeStandardDeck();
			Cards->ShuffleDeck(RNG, TEXT("Draws"));
		}
	}

	if (GameState)
	{
		GameState->ClearBoard();
		GameState->SetPlaysAndDiscards(3, 5);
		if (Cards)
		{
			GameState->SetPocket(Cards->Draw(2));
			GameState->AddCommunityCards(Cards->Draw(3)); // flop
		}
	}

	// Create HUD
	if (UWorld* World = GetWorld())
	{
		if (UBluffHUDWidget* HUD = CreateWidget<UBluffHUDWidget>(World, UBluffHUDWidget::StaticClass()))
		{
			HUD->AddToViewport(10);
			UE_LOG(LogTemp, Warning, TEXT("HUD Created and added to viewport!"));
			
			if (GameState)
			{
				FString Board;
				for (const FCard& C : GameState->GetPocket()) Board += CardToString(C) + TEXT(" ");
				Board += TEXT("| ");
				for (const FCard& C : GameState->GetCommunity()) Board += CardToString(C) + TEXT(" ");
				HUD->SetBoardText(FString::Printf(TEXT("Board: %s"), *Board));
				HUD->SetCountersText(FString::Printf(TEXT("Plays: %d  Discards: %d"), GameState->GetPlaysRemaining(), GameState->GetDiscardsRemaining()));
				HUD->SetScoreText(FString::Printf(TEXT("Score: %d"), GameState->GetLastFinalScore()));
				UE_LOG(LogTemp, Warning, TEXT("HUD initialized with board: %s"), *Board);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create HUD widget!"));
		}
	}

	// Auto-test: Simple HUD test first
	FTimerHandle TestTimer;
	GetWorldTimerManager().SetTimer(TestTimer, [this]()
	{
		UE_LOG(LogTemp, Warning, TEXT("=== HUD TEST ==="));
		
		// Test HUD update
		for (TObjectIterator<UBluffHUDWidget> It; It; ++It)
		{
			if (UBluffHUDWidget* HUD = *It)
			{
				HUD->SetScoreText(TEXT("HUD Test: Working!"));
				HUD->SetBoardText(TEXT("HUD Test: Board visible"));
				HUD->SetCountersText(TEXT("HUD Test: Counters visible"));
				UE_LOG(LogTemp, Warning, TEXT("HUD updated successfully!"));
			}
		}
		
		// Now test scoring
		UE_LOG(LogTemp, Warning, TEXT("Testing scoring systems..."));
		ScoreCurrentHand();
	}, 1.0f, false);
}

void ABluffGameModeBase::RevealTurn()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UCardSystem* Cards = GI->GetSubsystem<UCardSystem>())
		{
			if (ABluffGameState* GS = GetGameState<ABluffGameState>())
			{
				GS->AddCommunityCards(Cards->Draw(1));
				UE_LOG(LogTemp, Log, TEXT("Revealed Turn. Community count: %d"), GS->GetCommunity().Num());
			}
		}
	}
}

void ABluffGameModeBase::RevealRiver()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UCardSystem* Cards = GI->GetSubsystem<UCardSystem>())
		{
			if (ABluffGameState* GS = GetGameState<ABluffGameState>())
			{
				GS->AddCommunityCards(Cards->Draw(1));
				UE_LOG(LogTemp, Log, TEXT("Revealed River. Community count: %d"), GS->GetCommunity().Num());
			}
		}
	}
}

void ABluffGameModeBase::ScoreCurrentHand()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		UHandEvaluationSystem* EvalSys = GI->GetSubsystem<UHandEvaluationSystem>();
		UScoringSystem* ScoreSys = GI->GetSubsystem<UScoringSystem>();
		ABluffGameState* GS = GetGameState<ABluffGameState>();
		
		UE_LOG(LogTemp, Warning, TEXT("Debug - EvalSys: %s, ScoreSys: %s, GS: %s"), 
			EvalSys ? TEXT("Found") : TEXT("NULL"),
			ScoreSys ? TEXT("Found") : TEXT("NULL"),
			GS ? TEXT("Found") : TEXT("NULL"));
			
		if (!EvalSys || !ScoreSys || !GS)
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing systems for scoring."));
			return;
		}
		TArray<FCard> Candidates = GS->GetCommunity();
		Candidates.Append(GS->GetPocket());
		FHandEvaluationResult Eval = EvalSys->EvaluateBest5From7(Candidates);
		FScoreBreakdown Breakdown = ScoreSys->Score(Eval);
		GS->SetLastFinalScore(Breakdown.GetFinalScore());

		FString CardsStr;
		for (const FCard& C : Eval.ScoringCards)
		{
			CardsStr += CardToString(C) + TEXT(" ");
		}
		UE_LOG(LogTemp, Log, TEXT("Hand: %d Chips=%d Mult=%.2f BonusChips=%d BonusMult=%.2f Final=%d Cards=%s"),
			(int32)Eval.HandRank, Breakdown.BaseChips, Breakdown.BaseMult, Breakdown.BonusChips, Breakdown.BonusMult, Breakdown.GetFinalScore(), *CardsStr);

		// Update HUD if present
		for (TObjectIterator<UBluffHUDWidget> It; It; ++It)
		{
			if (UBluffHUDWidget* HUD = *It)
			{
				HUD->SetScoreText(FString::Printf(TEXT("Score: %d"), GS->GetLastFinalScore()));
			}
		}
	}
}

