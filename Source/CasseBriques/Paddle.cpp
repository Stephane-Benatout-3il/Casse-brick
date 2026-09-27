// Fill out your copyright notice in the Description page of Project Settings.

#include "Paddle.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "Ball.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

// Sets default values
APaddle::APaddle()
{
    // Set this pawn to call Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;

    PaddleMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("PaddleMesh")
    );

    RootComponent = PaddleMesh;
}

// Called when the game starts or when spawned
void APaddle::BeginPlay()
{
    Super::BeginPlay();

    // Sauvegarde la taille originale de la raquette
    OriginalPaddleScale = PaddleMesh->GetRelativeScale3D();

    if (APlayerController* PlayerController =
        Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer =
            PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                LocalPlayer->GetSubsystem<
                    UEnhancedInputLocalPlayerSubsystem>())
            {
                if (InputMapping)
                {
                    Subsystem->AddMappingContext(
                        InputMapping,
                        0
                    );
                }
            }
        }
    }
}

// Called every frame
void APaddle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APaddle::MoveHorizontal(const FInputActionValue& Value)
{
    const float MoveValue = Value.Get<float>();

    FVector NewLocation = GetActorLocation();

    NewLocation.X -=
        MoveValue *
        800.0f *
        GetWorld()->GetDeltaSeconds();

    FHitResult HitResult;

    SetActorLocation(
        NewLocation,
        true,
        &HitResult,
        ETeleportType::None
    );
}

void APaddle::LaunchBall()
{
    ABall* Ball = Cast<ABall>(
        UGameplayStatics::GetActorOfClass(
            this,
            ABall::StaticClass()
        )
    );

    if (Ball)
    {
        Ball->LaunchBall();
    }
}

void APaddle::PauseGame()
{
    // Si le jeu est déjà en pause, on le reprend
    if (UGameplayStatics::IsGamePaused(GetWorld()))
    {
        ResumeGame();
        return;
    }

    APlayerController* PlayerController =
        Cast<APlayerController>(GetController());

    if (!PlayerController || !PauseMenuWidgetClass)
    {
        return;
    }

    // Création du menu Pause
    PauseMenuWidget =
        CreateWidget<UUserWidget>(
            PlayerController,
            PauseMenuWidgetClass
        );

    if (!PauseMenuWidget)
    {
        return;
    }

    PauseMenuWidget->AddToViewport();

    // Met le jeu en pause
    UGameplayStatics::SetGamePaused(
        GetWorld(),
        true
    );

    // Affiche la souris
    PlayerController->bShowMouseCursor = true;

    // Permet d'utiliser le menu et les entrées du jeu
    FInputModeGameAndUI InputMode;

    InputMode.SetWidgetToFocus(
        PauseMenuWidget->TakeWidget()
    );

    PlayerController->SetInputMode(InputMode);
}

void APaddle::ResumeGame()
{
    APlayerController* PlayerController =
        Cast<APlayerController>(GetController());

    if (!PlayerController)
    {
        return;
    }

    // Retire le menu Pause
    if (PauseMenuWidget)
    {
        PauseMenuWidget->RemoveFromParent();
        PauseMenuWidget = nullptr;
    }

    // Reprend le jeu
    UGameplayStatics::SetGamePaused(
        GetWorld(),
        false
    );

    // Rend les contrôles au jeu
    FInputModeGameOnly InputMode;
    PlayerController->SetInputMode(InputMode);

    // Cache la souris
    PlayerController->bShowMouseCursor = false;
}

// Called to bind functionality to input
void APaddle::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent
)
{
    Super::SetupPlayerInputComponent(
        PlayerInputComponent
    );

    if (UEnhancedInputComponent* EnhancedInputComponent =
        Cast<UEnhancedInputComponent>(
            PlayerInputComponent
        ))
    {
        // Déplacement
        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(
                MoveAction,
                ETriggerEvent::Triggered,
                this,
                &APaddle::MoveHorizontal
            );
        }

        // Lancement de la balle
        if (LaunchAction)
        {
            EnhancedInputComponent->BindAction(
                LaunchAction,
                ETriggerEvent::Started,
                this,
                &APaddle::LaunchBall
            );
        }

        // Pause
        if (PauseAction)
        {
            EnhancedInputComponent->BindAction(
                PauseAction,
                ETriggerEvent::Started,
                this,
                &APaddle::PauseGame
            );
        }
    }
}

// ============================================================
// POWER-UP : AGRANDISSEMENT DE LA RAQUETTE
// ============================================================

void APaddle::ActivatePaddleSizePowerUp()
{
    FVector NewScale = OriginalPaddleScale;

    // La largeur de la raquette correspond à X
    NewScale.X *= PaddleSizeMultiplier;

    PaddleMesh->SetRelativeScale3D(NewScale);

    // Si le bonus était déjà actif,
    // on recommence la durée
    GetWorldTimerManager().ClearTimer(
        PaddleSizeTimerHandle
    );

    GetWorldTimerManager().SetTimer(
        PaddleSizeTimerHandle,
        this,
        &APaddle::ResetPaddleSize,
        PaddleSizeDuration,
        false
    );
}

void APaddle::ResetPaddleSize()
{
    PaddleMesh->SetRelativeScale3D(
        OriginalPaddleScale
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("POWER-UP : RAQUETTE TAILLE NORMALE")
    );
}

// ============================================================
// POWER-UP : MULTIBALL
// ============================================================

void APaddle::ActivateMultiballPowerUp()
{
    if (!BallClass)
    {
        return;
    }

    // Recherche une balle actuellement présente
    ABall* CurrentBall = Cast<ABall>(
        UGameplayStatics::GetActorOfClass(
            this,
            ABall::StaticClass()
        )
    );

    if (!CurrentBall)
    {
        return;
    }

    const FVector SpawnLocation =
        CurrentBall->GetActorLocation();

    // ========================================================
    // Balle supplémentaire gauche
    // ========================================================

    FVector Ball2Location = SpawnLocation;
    Ball2Location.X -= 50.0f;

    ABall* Ball2 = GetWorld()->SpawnActor<ABall>(
        BallClass,
        Ball2Location,
        FRotator::ZeroRotator
    );

    if (Ball2)
    {
        // Ce n'est pas la balle principale
        Ball2->SetIsMainBall(false);

        Ball2->LaunchBallInDirection(
            FVector(
                -0.7f,
                1.0f,
                0.0f
            )
        );
    }

    // ========================================================
    // Balle supplémentaire droite
    // ========================================================

    FVector Ball3Location = SpawnLocation;
    Ball3Location.X += 50.0f;

    ABall* Ball3 = GetWorld()->SpawnActor<ABall>(
        BallClass,
        Ball3Location,
        FRotator::ZeroRotator
    );

    if (Ball3)
    {
        // Ce n'est pas la balle principale
        Ball3->SetIsMainBall(false);

        Ball3->LaunchBallInDirection(
            FVector(
                0.7f,
                1.0f,
                0.0f
            )
        );
    }
}