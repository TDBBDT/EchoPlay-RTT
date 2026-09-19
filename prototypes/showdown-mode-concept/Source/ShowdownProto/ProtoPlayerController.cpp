// PROTOTYPE - NOT FOR PRODUCTION
// Question: Can UE 5.6 implement Desperados III-style Showdown Mode with
//           frame-accurate command execution and a fully frozen world during planning?
// Date: 2026-09-19

#include "ProtoPlayerController.h"
#include "ProtoGameMode.h"
#include "ProtoEnemyUnit.h"
#include "EngineUtils.h"

AProtoPlayerController::AProtoPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	// There is no pawn. Without this, the controller keeps resetting the view
	// target to itself and the tactical camera never takes effect.
	bAutoManageActiveCameraTarget = false;
}

void AProtoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent)
	{
		return;
	}

	InputComponent->BindKey(EKeys::SpaceBar, IE_Pressed, this, &AProtoPlayerController::OnPausePressed);
	InputComponent->BindKey(EKeys::E, IE_Pressed, this, &AProtoPlayerController::OnExecutePressed);
	InputComponent->BindKey(EKeys::One, IE_Pressed, this, &AProtoPlayerController::OnSelect1Pressed);
	InputComponent->BindKey(EKeys::Two, IE_Pressed, this, &AProtoPlayerController::OnSelect2Pressed);
	InputComponent->BindKey(EKeys::V, IE_Pressed, this, &AProtoPlayerController::OnToggleGatePressed);
	InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &AProtoPlayerController::OnLeftClickPressed);
}

void AProtoPlayerController::OnPausePressed()
{
	AProtoGameMode* GM = AProtoGameMode::Get(this);
	if (!GM)
	{
		return;
	}

	if (GM->bIsPaused)
	{
		GM->UnpauseAndDiscard();
	}
	else
	{
		GM->SetPaused(true);
		GM->StatusMessage = TEXT("Time FROZEN. Click ground to MOVE, click enemy to KILL. E to execute.");
	}
}

void AProtoPlayerController::OnExecutePressed()
{
	if (AProtoGameMode* GM = AProtoGameMode::Get(this))
	{
		GM->ExecuteAll();
	}
}

void AProtoPlayerController::OnSelect1Pressed()
{
	if (AProtoGameMode* GM = AProtoGameMode::Get(this))
	{
		GM->SelectUnit(0);
	}
}

void AProtoPlayerController::OnSelect2Pressed()
{
	if (AProtoGameMode* GM = AProtoGameMode::Get(this))
	{
		GM->SelectUnit(1);
	}
}

void AProtoPlayerController::OnToggleGatePressed()
{
	if (AProtoGameMode* GM = AProtoGameMode::Get(this))
	{
		GM->TogglePauseGate();
	}
}

void AProtoPlayerController::OnLeftClickPressed()
{
	AProtoGameMode* GM = AProtoGameMode::Get(this);
	if (!GM)
	{
		return;
	}

	if (!GM->bIsPaused)
	{
		GM->StatusMessage = TEXT("Time is running. Press SPACE to freeze before queueing.");
		return;
	}

	FVector WorldPoint;
	if (!TraceGroundUnderCursor(WorldPoint))
	{
		return;
	}

	AActor* Enemy = FindEnemyNear(WorldPoint, 180.f);
	GM->QueueCommandForSelected(WorldPoint, Enemy);
}

bool AProtoPlayerController::TraceGroundUnderCursor(FVector& OutWorldPoint) const
{
	FVector Origin, Dir;
	if (!DeprojectMousePositionToWorld(Origin, Dir))
	{
		return false;
	}

	if (FMath::IsNearlyZero(Dir.Z, 1e-4f))
	{
		return false;
	}

	const float T = -Origin.Z / Dir.Z;
	if (T <= 0.f)
	{
		return false;
	}

	OutWorldPoint = Origin + Dir * T;
	return true;
}

AActor* AProtoPlayerController::FindEnemyNear(const FVector& WorldPoint, float MaxDist) const
{
	AActor* Best = nullptr;
	float BestDist = MaxDist;

	for (TActorIterator<AProtoEnemyUnit> It(GetWorld()); It; ++It)
	{
		AProtoEnemyUnit* E = *It;
		if (!E || E->bDead)
		{
			continue;
		}

		FVector Delta = E->GetActorLocation() - WorldPoint;
		Delta.Z = 0.f;
		const float D = Delta.Size();

		if (D < BestDist)
		{
			BestDist = D;
			Best = E;
		}
	}

	return Best;
}
