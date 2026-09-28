// Fill out your copyright notice in the Description page of Project Settings.

#include "BreakoutGameMode.h"
#include "BreakoutGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Ball.h"
#include "Kismet/GameplayStatics.h"

ABreakoutGameMode::ABreakoutGameMode()
{
    Score = 0;
    Lives = 3;
}

void ABreakoutGameMode::BeginPlay()
{
    Super::BeginPlay();

    UBreakoutGameInstance* GameInstance =
        Cast<UBreakoutGameInstance>(GetGameInstance());

    if (GameInstance)
    {
        Score = GameInstance->SavedScore;
        Lives = GameInstance->SavedLives;
    }
}

void ABreakoutGameMode::AddScore(int32 Amount)
{
    const int32 PointsWithCombo = Amount * ComboMultiplier;

    Score += PointsWithCombo;

    if (UBreakoutGameInstance* GameInstance =
        Cast<UBreakoutGameInstance>(GetGameInstance()))
    {
        GameInstance->SavedScore = Score;
    }
}

void ABreakoutGameMode::IncreaseCombo()
{
    if (ComboMultiplier < 5)
    {
        ComboMultiplier++;
    }
}

void ABreakoutGameMode::ResetCombo()
{
    ComboMultiplier = 1;
}

void ABreakoutGameMode::LoseLife()
{
    if (Lives <= 0)
    {
        return;
    }

    Lives--;

    if (UBreakoutGameInstance* GameInstance =
        Cast<UBreakoutGameInstance>(GetGameInstance()))
    {
        GameInstance->SavedLives = Lives;
    }

    if (Lives == 0)
    {
        GameOver();
    }
}

void ABreakoutGameMode::GameOver()
{
    // Évite de déclencher plusieurs fois le Game Over
    if (bIsGameOver)
    {
        return;
    }

    bIsGameOver = true;

    // Création du menu Game Over
    if (GameOverWidgetClass)
    {
        UUserWidget* GameOverWidget =
            CreateWidget<UUserWidget>(
                GetWorld(),
                GameOverWidgetClass
            );

        if (GameOverWidget)
        {
            GameOverWidget->AddToViewport();

            APlayerController* PlayerController =
                GetWorld()->GetFirstPlayerController();

            if (PlayerController)
            {
                // Affiche le curseur
                PlayerController->bShowMouseCursor = true;

                // Donne le contrôle au menu Game Over
                FInputModeUIOnly InputMode;

                InputMode.SetWidgetToFocus(
                    GameOverWidget->TakeWidget()
                );

                InputMode.SetLockMouseToViewportBehavior(
                    EMouseLockMode::DoNotLock
                );

                PlayerController->SetInputMode(InputMode);
            }
        }
    }
}

void ABreakoutGameMode::SetBrickCount(int32 Count)
{
    RemainingBricks = Count;
    bIsLevelComplete = false;
}

void ABreakoutGameMode::BrickDestroyed()
{
    if (RemainingBricks <= 0 || bIsLevelComplete)
    {
        return;
    }

    RemainingBricks--;

    if (RemainingBricks == 0)
    {
        LevelComplete();
    }
}

void ABreakoutGameMode::LevelComplete()
{
    if (bIsLevelComplete)
    {
        return;
    }

    bIsLevelComplete = true;

    // ---------------------------------------------------------
    // ARRÊT DES BALLES À LA FIN DU NIVEAU
    // ---------------------------------------------------------

    TArray<AActor*> Balls;

    UGameplayStatics::GetAllActorsOfClass(
        this,
        ABall::StaticClass(),
        Balls
    );

    for (AActor* Actor : Balls)
    {
        ABall* Ball = Cast<ABall>(Actor);

        if (!Ball)
        {
            continue;
        }

        // Balle principale :
        // arrêt et repositionnement sur la raquette
        if (Ball->IsMainBall())
        {
            Ball->SetIsInPlay(false);
            Ball->ResetBall();
        }
        else
        {
            // Les balles secondaires du multiball
            // sont supprimées à la victoire
            Ball->Destroy();
        }
    }

    // ---------------------------------------------------------
    // AFFICHAGE DU MENU DE VICTOIRE
    // ---------------------------------------------------------

    if (LevelCompleteWidgetClass)
    {
        UUserWidget* LevelCompleteWidget =
            CreateWidget<UUserWidget>(
                GetWorld(),
                LevelCompleteWidgetClass
            );

        if (LevelCompleteWidget)
        {
            LevelCompleteWidget->AddToViewport();

            APlayerController* PlayerController =
                GetWorld()->GetFirstPlayerController();

            if (PlayerController)
            {
                PlayerController->bShowMouseCursor = true;

                FInputModeUIOnly InputMode;

                InputMode.SetWidgetToFocus(
                    LevelCompleteWidget->TakeWidget()
                );

                InputMode.SetLockMouseToViewportBehavior(
                    EMouseLockMode::DoNotLock
                );

                PlayerController->SetInputMode(InputMode);
            }
        }
    }
}