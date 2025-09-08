#include "UI/BluffHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"

void UBluffHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UVerticalBox* Root = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
	WidgetTree->RootWidget = Root;

	BoardText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	CountersText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	ScoreText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

	BoardText->SetText(FText::FromString(TEXT("Board: -")));
	CountersText->SetText(FText::FromString(TEXT("Counters: -")));
	ScoreText->SetText(FText::FromString(TEXT("Score: -")));
	
	// Make text more visible
	BoardText->SetColorAndOpacity(FLinearColor::White);
	CountersText->SetColorAndOpacity(FLinearColor::Yellow);
	ScoreText->SetColorAndOpacity(FLinearColor::Green);
	
	// Set font size
	FSlateFontInfo FontInfo = BoardText->GetFont();
	FontInfo.Size = 24;
	BoardText->SetFont(FontInfo);
	CountersText->SetFont(FontInfo);
	ScoreText->SetFont(FontInfo);

	Root->AddChildToVerticalBox(BoardText);
	Root->AddChildToVerticalBox(CountersText);
	Root->AddChildToVerticalBox(ScoreText);
}

void UBluffHUDWidget::SetBoardText(const FString& InText)
{
	if (BoardText) BoardText->SetText(FText::FromString(InText));
}

void UBluffHUDWidget::SetCountersText(const FString& InText)
{
	if (CountersText) CountersText->SetText(FText::FromString(InText));
}

void UBluffHUDWidget::SetScoreText(const FString& InText)
{
	if (ScoreText) ScoreText->SetText(FText::FromString(InText));
}
