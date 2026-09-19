// PROTOTYPE - NOT FOR PRODUCTION
// Question: Can UE 5.6 implement Desperados III-style Showdown Mode with
//           frame-accurate command execution and a fully frozen world during planning?
// Date: 2026-09-19

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProtoUnit.generated.h"

/**
 * Base unit. No mesh, no materials — everything is debug-drawn so the prototype
 * needs zero binary assets and can be dropped into an empty level.
 *
 * Movement is deliberately manual (not CharacterMovementComponent) because a real
 * RTT needs deterministic, frame-exact locomotion — and because it lets us observe
 * exactly what CustomTimeDilation=0 does and does not freeze.
 */
UCLASS()
class AProtoUnit : public AActor
{
	GENERATED_BODY()

public:
	AProtoUnit();

	virtual void Tick(float DeltaTime) override;

	/** Color used for debug drawing. */
	FLinearColor DebugColor = FLinearColor(1.f, 1.f, 1.f);

	float Radius = 45.f;
	float Height = 170.f;
	float MoveSpeed = 420.f;

	/** Unit facing, used for vision cones and the facing tick-mark. */
	FVector FacingDir = FVector(1.f, 0.f, 0.f);

	bool bDead = false;

	/** Torso/eye height — the point vision checks originate from. */
	FVector GetEyeLocation() const;

	/**
	 * Move toward Target at MoveSpeed.
	 * Returns true when within arrival tolerance.
	 * Returns false immediately when DeltaTime <= 0 (i.e. while frozen).
	 */
	bool StepToward(const FVector& Target, float DeltaTime);

	void DrawSelf() const;
};
