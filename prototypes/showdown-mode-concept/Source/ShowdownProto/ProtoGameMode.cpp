// PROTOTYPE - NOT FOR PRODUCTION
// Question: Can UE 5.6 implement Desperados III-style Showdown Mode with
//           frame-accurate command execution and a fully frozen world during planning?
// Date: 2026-09-19

#include "ProtoGameMode.h"
#include "ProtoPlayerUnit.h"
#include "ProtoEnemyUnit.h"
#include "ProtoPlayerController.h"
#include "ProtoHUD.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

AProtoGameMode::AProtoGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	DefaultPawnClass = nullptr; // no pawn — we look through a fixed tactical camera
	PlayerControllerClass = AProtoPlayerController::StaticClass();
	HUDClass = AProtoHUD::StaticClass();
}

void AProtoGameMode::BeginPlay()
{
	Super::BeginPlay();
	BuildWorld();
}

void AProtoGameMode::BuildWorld()
{
	UWorld* W = GetWorld();
	if (!W)
	{
		return;
	}

	FActorSpawnParameters SP;
	SP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// --- Tactical camera: orthographic top-down, classic RTT framing ---
	ACameraActor* Cam = W->SpawnActor<ACameraActor>(
		ACameraActor::StaticClass(),
		FVector(0.f, 0.f, 3000.f),
		FRotator(-90.f, 0.f, 0.f),
		SP);

	if (Cam)
	{
		if (UCameraComponent* CamComp = Cam->GetCameraComponent())
		{
			// Orthographic top-down framing — standard for RTT, and essential here
			// because the cursor is deprojected straight onto the Z=0 plane.
			CamComp->ProjectionMode = ECameraProjectionMode::Orthographic;
			CamComp->SetOrthoWidth(5200.f);
			CamComp->SetOrthoNearClipPlane(10.f);
			CamComp->SetOrthoFarClipPlane(10000.f);
		}
	}

	// --- Player units ---
	AProtoPlayerUnit* P1 = W->SpawnActor<AProtoPlayerUnit>(
		AProtoPlayerUnit::StaticClass(),
		FVector(-900.f, -350.f, 90.f), FRotator::ZeroRotator, SP);

	AProtoPlayerUnit* P2 = W->SpawnActor<AProtoPlayerUnit>(
		AProtoPlayerUnit::StaticClass(),
		FVector(-900.f,  350.f, 90.f), FRotator::ZeroRotator, SP);

	if (P1)
	{
		P1->DebugColor = FLinearColor(0.20f, 0.55f, 1.0f);
		P1->PlayerIndex = 0;
		PlayerUnits.Add(P1);
	}
	if (P2)
	{
		P2->DebugColor = FLinearColor(0.20f, 1.0f, 0.55f);
		P2->PlayerIndex = 1;
		PlayerUnits.Add(P2);
	}

	// --- Enemy ---
	AProtoEnemyUnit* E1 = W->SpawnActor<AProtoEnemyUnit>(
		AProtoEnemyUnit::StaticClass(),
		FVector(300.f, 0.f, 90.f), FRotator::ZeroRotator, SP);

	if (E1)
	{
		E1->InitPatrol();
		EnemyUnits.Add(E1);
	}

	// --- Point the player at the tactical camera ---
	if (APlayerController* PC = W->GetFirstPlayerController())
	{
		if (Cam)
		{
			PC->SetViewTargetWithBlend(Cam, 0.f);
		}
	}

	ApplyTimeStateToUnits();

	StatusMessage = TEXT("Press SPACE to freeze time, then click to queue commands.");
}

void AProtoGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawGroundGrid();
}

void AProtoGameMode::DrawGroundGrid() const
{
	UWorld* W = GetWorld();
	if (!W)
	{
		return;
	}

	const FColor GridColor(70, 70, 70);
	const float Extent = 2500.f;
	const float Step = 250.f;

	for (float X = -Extent; X <= Extent; X += Step)
	{
		DrawDebugLine(W, FVector(X, -Extent, 0.f), FVector(X, Extent, 0.f),
			GridColor, false, 0.f, 0, 1.f);
	}
	for (float Y = -Extent; Y <= Extent; Y += Step)
	{
		DrawDebugLine(W, FVector(-Extent, Y, 0.f), FVector(Extent, Y, 0.f),
			GridColor, false, 0.f, 0, 1.f);
	}
}

void AProtoGameMode::ApplyTimeStateToUnits()
{
	const float Dilation = bIsPaused ? 0.f : 1.f;

	for (AProtoPlayerUnit* P : PlayerUnits)
	{
		if (P)
		{
			P->CustomTimeDilation = Dilation;
		}
	}

	for (AProtoEnemyUnit* E : EnemyUnits)
	{
		if (E)
		{
			E->CustomTimeDilation = Dilation;
			E->bWorldPaused = bIsPaused;
			E->bPauseGateEnabled = bPauseGateEnabled;
		}
	}
}

void AProtoGameMode::SetPaused(bool bPause)
{
	bIsPaused = bPause;
	ApplyTimeStateToUnits();
}

void AProtoGameMode::TogglePauseGate()
{
	bPauseGateEnabled = !bPauseGateEnabled;
	ApplyTimeStateToUnits();

	StatusMessage = bPauseGateEnabled
		? TEXT("Vision gate ON — enemy vision is explicitly frozen while paused.")
		: TEXT("Vision gate OFF — showing raw CustomTimeDilation behaviour.");
}

void AProtoGameMode::UnpauseAndDiscard()
{
	for (AProtoPlayerUnit* P : PlayerUnits)
	{
		if (P)
		{
			P->ClearQueue();
		}
	}
	SetPaused(false);
	StatusMessage = TEXT("Resumed without executing — queued commands discarded.");
}

void AProtoGameMode::ExecuteAll()
{
	SetPaused(false);

	int32 Started = 0;
	for (AProtoPlayerUnit* P : PlayerUnits)
	{
		if (P && P->Queue.Num() > 0)
		{
			P->BeginExecution();
			++Started;
		}
	}

	++ExecuteCount;
	StatusMessage = FString::Printf(TEXT("EXECUTE #%d — %d unit(s) acting."), ExecuteCount, Started);
}

void AProtoGameMode::SelectUnit(int32 Index)
{
	if (PlayerUnits.IsValidIndex(Index))
	{
		SelectedIndex = Index;
		StatusMessage = FString::Printf(TEXT("Selected unit %d."), Index + 1);
	}
}

AProtoPlayerUnit* AProtoGameMode::GetSelected() const
{
	return PlayerUnits.IsValidIndex(SelectedIndex) ? PlayerUnits[SelectedIndex] : nullptr;
}

void AProtoGameMode::QueueCommandForSelected(const FVector& WorldLocation, AActor* TargetActor)
{
	AProtoPlayerUnit* P = GetSelected();
	if (!P)
	{
		return;
	}

	if (TargetActor)
	{
		P->QueueKill(TargetActor);
		StatusMessage = FString::Printf(TEXT("Unit %d: KILL queued."), SelectedIndex + 1);
	}
	else
	{
		P->QueueMove(WorldLocation);
		StatusMessage = FString::Printf(TEXT("Unit %d: MOVE queued."), SelectedIndex + 1);
	}
}

float AProtoGameMode::ComputeSimultaneityDriftMs() const
{
	if (PlayerUnits.Num() < 2)
	{
		return -1.f;
	}

	const AProtoPlayerUnit* A = PlayerUnits[0];
	const AProtoPlayerUnit* B = PlayerUnits[1];
	if (!A || !B)
	{
		return -1.f;
	}
	if (A->LastCommandStartTime < 0.f || B->LastCommandStartTime < 0.f)
	{
		return -1.f;
	}

	// Only meaningful when both units are working on the same sequence step.
	if (A->LastCommandStartSeq != B->LastCommandStartSeq)
	{
		return -1.f;
	}

	return FMath::Abs(A->LastCommandStartTime - B->LastCommandStartTime) * 1000.f;
}

int32 AProtoGameMode::GetDetectionsWhilePaused() const
{
	int32 Total = 0;
	for (const AProtoEnemyUnit* E : EnemyUnits)
	{
		if (E)
		{
			Total += E->DetectEventsWhilePaused;
		}
	}
	return Total;
}

AProtoGameMode* AProtoGameMode::Get(const UObject* WorldContext)
{
	if (!WorldContext || !GEngine)
	{
		return nullptr;
	}

	UWorld* W = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	return W ? Cast<AProtoGameMode>(W->GetAuthGameMode()) : nullptr;
}
