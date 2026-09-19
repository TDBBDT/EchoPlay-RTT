// PROTOTYPE - NOT FOR PRODUCTION
// Question: Can UE 5.6 implement Desperados III-style Showdown Mode with
//           frame-accurate command execution and a fully frozen world during planning?
// Date: 2026-09-19

#include "ProtoHUD.h"
#include "ProtoGameMode.h"
#include "ProtoPlayerUnit.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"

void AProtoHUD::DrawHUD()
{
	Super::DrawHUD();

	AProtoGameMode* GM = AProtoGameMode::Get(this);
	if (!GM)
	{
		return;
	}

	UFont* Font = GEngine ? GEngine->GetSmallFont() : nullptr;
	if (!Font)
	{
		return;
	}

	float Y = 24.f;
	const float X = 24.f;
	const float LH = 15.f;

	auto Line = [&](const FString& Text, const FLinearColor& Color)
	{
		DrawText(Text, Color, X, Y, Font, 1.15f, false);
		Y += LH;
	};
	auto Gap = [&](float Amount) { Y += Amount; };

	// --- Header ---
	Line(TEXT("=== SHOWDOWN MODE PROTOTYPE ===  (PROTOTYPE - NOT FOR PRODUCTION)"),
		FLinearColor(0.6f, 0.6f, 0.6f));
	Gap(6.f);

	// --- Time state ---
	Line(GM->bIsPaused
		? TEXT("TIME: FROZEN  —  queuing commands")
		: TEXT("TIME: RUNNING"),
		GM->bIsPaused ? FLinearColor(1.f, 0.85f, 0.2f) : FLinearColor(0.4f, 1.f, 0.4f));

	// --- Vision gate status (the core technical probe) ---
	Line(GM->bPauseGateEnabled
		? TEXT("Vision gate [V]: ON   — enemy vision explicitly frozen while paused")
		: TEXT("Vision gate [V]: OFF  — raw CustomTimeDilation only"),
		GM->bPauseGateEnabled ? FLinearColor(0.4f, 1.f, 0.4f) : FLinearColor(1.f, 0.3f, 0.3f));

	const int32 Leaks = GM->GetDetectionsWhilePaused();
	Line(FString::Printf(TEXT("Detections while frozen: %d %s"),
		Leaks,
		Leaks == 0 ? TEXT("(no leak)") : TEXT("<-- LEAK: world was not truly frozen")),
		Leaks == 0 ? FLinearColor(0.4f, 1.f, 0.4f) : FLinearColor(1.f, 0.3f, 0.3f));

	Gap(6.f);

	// --- Per-unit command queues ---
	for (int32 i = 0; i < GM->PlayerUnits.Num(); ++i)
	{
		AProtoPlayerUnit* P = GM->PlayerUnits[i];
		if (!P)
		{
			continue;
		}

		const bool bSelected = (i == GM->SelectedIndex);
		const FLinearColor HeaderColor = bSelected
			? FLinearColor(1.f, 1.f, 1.f)
			: FLinearColor(0.55f, 0.55f, 0.55f);

		Line(FString::Printf(TEXT("%s Unit %d%s — %d command(s)%s"),
			bSelected ? TEXT(">") : TEXT(" "),
			i + 1,
			bSelected ? TEXT(" [SELECTED]") : TEXT(""),
			P->Queue.Num(),
			P->bExecuting ? TEXT("  [EXECUTING]") : TEXT("")),
			HeaderColor);

		for (int32 c = 0; c < P->Queue.Num(); ++c)
		{
			const FProtoCommand& Cmd = P->Queue[c];
			const TCHAR* TypeName = (Cmd.Type == EProtoCommandType::Kill) ? TEXT("KILL") : TEXT("MOVE");

			FString Where;
			if (Cmd.Type == EProtoCommandType::Kill)
			{
				Where = Cmd.TargetActor ? Cmd.TargetActor->GetName() : TEXT("<dead>");
			}
			else
			{
				Where = FString::Printf(TEXT("(%.0f, %.0f)"), Cmd.TargetLocation.X, Cmd.TargetLocation.Y);
			}

			const FString Stamp = Cmd.bStarted
				? FString::Printf(TEXT("@ %.4fs"), Cmd.StartTime)
				: TEXT("@  --      ");

			Line(FString::Printf(TEXT("    #%d  %-4s -> %-14s  %s"),
				c, TypeName, *Where, *Stamp),
				Cmd.bStarted ? FLinearColor(0.9f, 0.9f, 0.5f) : FLinearColor(0.7f, 0.7f, 0.7f));
		}
		Gap(4.f);
	}

	Gap(4.f);

	// --- The measurable signal ---
	const float Drift = GM->ComputeSimultaneityDriftMs();
	if (Drift < 0.f)
	{
		Line(TEXT("Simultaneity drift: n/a (both units must start the same command index)"),
			FLinearColor(0.6f, 0.6f, 0.6f));
	}
	else
	{
		Line(FString::Printf(TEXT("Simultaneity drift: %.3f ms  %s"),
			Drift,
			Drift <= 0.001f ? TEXT("PASS (same frame)") : TEXT("DRIFT DETECTED")),
			Drift <= 0.001f ? FLinearColor(0.4f, 1.f, 0.4f) : FLinearColor(1.f, 0.6f, 0.2f));
	}

	Line(FString::Printf(TEXT("Execute count: %d"), GM->ExecuteCount),
		FLinearColor(0.6f, 0.6f, 0.6f));

	Gap(8.f);

	// --- Status message ---
	Line(GM->StatusMessage, FLinearColor(0.9f, 0.9f, 1.f));

	Gap(8.f);

	// --- Controls ---
	Line(TEXT("CONTROLS"), FLinearColor(0.6f, 0.6f, 0.6f));
	Line(TEXT("  SPACE   freeze / unfreeze time (unfreezing discards the queue)"),
		FLinearColor(0.75f, 0.75f, 0.75f));
	Line(TEXT("  1 / 2   select unit 1 / unit 2"),
		FLinearColor(0.75f, 0.75f, 0.75f));
	Line(TEXT("  LMB     while frozen: ground = MOVE, enemy = KILL"),
		FLinearColor(0.75f, 0.75f, 0.75f));
	Line(TEXT("  E       execute all queued commands (time resumes)"),
		FLinearColor(0.75f, 0.75f, 0.75f));
	Line(TEXT("  V       toggle the vision pause-gate (debug probe)"),
		FLinearColor(0.75f, 0.75f, 0.75f));
}
