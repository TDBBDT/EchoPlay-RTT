// PROTOTYPE - NOT FOR PRODUCTION
// Question: Can UE 5.6 implement Desperados III-style Showdown Mode with
//           frame-accurate command execution and a fully frozen world during planning?
// Date: 2026-09-19

#include "ProtoUnit.h"
#include "DrawDebugHelpers.h"

namespace
{
	constexpr float ArriveTolerance = 18.f;
}

AProtoUnit::AProtoUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	// Nothing in this prototype collides — clicks are resolved by deprojecting the
	// mouse to the Z=0 plane, not by trace hits.
	SetActorEnableCollision(false);
}

FVector AProtoUnit::GetEyeLocation() const
{
	return GetActorLocation() + FVector(0.f, 0.f, Height * 0.7f);
}

bool AProtoUnit::StepToward(const FVector& Target, float DeltaTime)
{
	// DeltaTime is 0 while the unit is frozen (CustomTimeDilation = 0).
	// Returning early here is what makes movement genuinely stop.
	if (DeltaTime <= 0.f)
	{
		return false;
	}

	const FVector Loc = GetActorLocation();
	FVector Delta = Target - Loc;
	Delta.Z = 0.f;
	const float Dist = Delta.Size();

	if (Dist <= ArriveTolerance)
	{
		return true;
	}

	const FVector Dir = Delta / Dist;
	FacingDir = Dir;

	const float Step = FMath::Min(MoveSpeed * DeltaTime, Dist);
	SetActorLocation(Loc + Dir * Step);
	return false;
}

void AProtoUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawSelf();
}

void AProtoUnit::DrawSelf() const
{
	if (bDead)
	{
		return;
	}

	const FVector Loc = GetActorLocation();
	const FColor Body = DebugColor.ToFColor(true);

	DrawDebugCylinder(GetWorld(), Loc, Loc + FVector(0.f, 0.f, Height), Radius, 16,
		Body, false, 0.f, 0, 3.f);

	// Facing tick-mark
	const FVector Mid = Loc + FVector(0.f, 0.f, Height * 0.5f);
	DrawDebugLine(GetWorld(), Mid, Mid + FacingDir * (Radius * 2.2f),
		FColor::White, false, 0.f, 0, 3.f);
}
