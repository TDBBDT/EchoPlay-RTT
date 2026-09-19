// PROTOTYPE - NOT FOR PRODUCTION
// Question: Can UE 5.6 implement Desperados III-style Showdown Mode with
//           frame-accurate command execution and a fully frozen world during planning?
// Date: 2026-09-19

#include "ProtoEnemyUnit.h"
#include "ProtoPlayerUnit.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"

AProtoEnemyUnit::AProtoEnemyUnit()
{
	DebugColor = FLinearColor(1.f, 0.25f, 0.25f);
	MoveSpeed = 260.f; // patrol slower than player units run
}

void AProtoEnemyUnit::InitPatrol()
{
	const FVector Base = GetActorLocation();

	PatrolPoints.Empty();
	PatrolPoints.Add(Base + FVector(-500.f, -700.f, 0.f));
	PatrolPoints.Add(Base + FVector(-500.f,  700.f, 0.f));
	PatrolPoints.Add(Base + FVector( 700.f,  700.f, 0.f));
	PatrolPoints.Add(Base + FVector( 700.f, -700.f, 0.f));

	PatrolIndex = 0;
	WaitTimer = 0.f;
}

void AProtoEnemyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // base draws the body

	CheckVision();
	DrawVisionCone();

	if (bDead)
	{
		return;
	}

	if (PatrolPoints.Num() == 0)
	{
		return;
	}

	if (WaitTimer > 0.f)
	{
		WaitTimer -= DeltaTime;
		return;
	}

	if (StepToward(PatrolPoints[PatrolIndex], DeltaTime))
	{
		PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
		WaitTimer = WaitDuration;
	}
}

void AProtoEnemyUnit::CheckVision()
{
	bDetectedThisFrame = false;

	if (bDead || !bVisionEnabled)
	{
		return;
	}

	// ---------------------------------------------------------------------
	// THE CENTRAL TEST OF THIS PROTOTYPE.
	//
	// CustomTimeDilation = 0 does NOT stop this function. Nothing here reads
	// DeltaTime, so the engine has no reason to skip it. Without this explicit
	// gate the guard keeps detecting players while the world appears frozen.
	//
	// Toggle with V to see both behaviours.
	// ---------------------------------------------------------------------
	if (bWorldPaused && bPauseGateEnabled)
	{
		return;
	}

	const FVector Eye = GetEyeLocation();

	for (TActorIterator<AProtoPlayerUnit> It(GetWorld()); It; ++It)
	{
		AProtoPlayerUnit* P = *It;
		if (!P || P->bDead)
		{
			continue;
		}

		FVector To = P->GetEyeLocation() - Eye;
		const float Dist = To.Size();
		if (Dist > VisionRange || Dist <= KINDA_SMALL_NUMBER)
		{
			continue;
		}

		To /= Dist;
		const float CosAngle = FVector::DotProduct(FacingDir, To);
		const float AngleDeg = FMath::RadiansToDegrees(
			FMath::Acos(FMath::Clamp(CosAngle, -1.f, 1.f)));

		if (AngleDeg > VisionHalfAngleDeg)
		{
			continue;
		}

		// Detected.
		bDetectedThisFrame = true;
		bAlerted = true;
		++DetectEventsTotal;
		if (bWorldPaused)
		{
			++DetectEventsWhilePaused;
		}
		LastDetectTime = GetWorld()->GetTimeSeconds();
	}
}

void AProtoEnemyUnit::DrawVisionCone() const
{
	if (bDead || !bVisionEnabled)
	{
		return;
	}

	FColor Col = FColor::Green;
	if (bDetectedThisFrame)
	{
		Col = FColor::Yellow;
	}
	if (bWorldPaused && !bPauseGateEnabled)
	{
		// Make the failure mode visually obvious.
		Col = FColor::Magenta;
	}

	const float HalfAngleRad = FMath::DegreesToRadians(VisionHalfAngleDeg);

	DrawDebugCone(GetWorld(), GetEyeLocation(), FacingDir, VisionRange,
		HalfAngleRad, HalfAngleRad, 24, Col, false, 0.f, 0, 2.f);
}
