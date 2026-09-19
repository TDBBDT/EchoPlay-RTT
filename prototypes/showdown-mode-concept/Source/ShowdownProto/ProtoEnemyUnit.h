// PROTOTYPE - NOT FOR PRODUCTION
// Question: Can UE 5.6 implement Desperados III-style Showdown Mode with
//           frame-accurate command execution and a fully frozen world during planning?
// Date: 2026-09-19

#pragma once

#include "CoreMinimal.h"
#include "ProtoUnit.h"
#include "ProtoEnemyUnit.generated.h"

/**
 * Patrolling guard with a visible vision cone.
 *
 * This class carries the prototype's central technical question. Enemy vision is
 * a dot-product/LOS test that does NOT consume DeltaTime. Setting
 * CustomTimeDilation = 0 stops everything that is delta-driven (movement, timers,
 * animation) but does nothing to logic that simply runs every Tick. So the vision
 * check must be gated explicitly.
 *
 * Press V in-game to toggle that gate and watch the difference directly.
 */
UCLASS()
class AProtoEnemyUnit : public AProtoUnit
{
	GENERATED_BODY()

public:
	AProtoEnemyUnit();

	virtual void Tick(float DeltaTime) override;

	// --- Patrol ---
	UPROPERTY() TArray<FVector> PatrolPoints;
	int32 PatrolIndex = 0;
	float WaitTimer = 0.f;
	float WaitDuration = 1.0f;

	// --- Vision ---
	float VisionRange = 950.f;
	float VisionHalfAngleDeg = 42.f;

	bool bAlerted = false;
	bool bDetectedThisFrame = false;
	int32 DetectEventsWhilePaused = 0;
	int32 DetectEventsTotal = 0;
	float LastDetectTime = -1.f;

	// --- Injected by the GameMode ---
	/** True while the Showdown timer is stopped. */
	bool bWorldPaused = false;
	/** When true, the vision check is explicitly skipped while paused. */
	bool bPauseGateEnabled = true;
	/** Master switch for vision processing (debug). */
	bool bVisionEnabled = true;

	void InitPatrol();
	void CheckVision();
	void DrawVisionCone() const;
};
