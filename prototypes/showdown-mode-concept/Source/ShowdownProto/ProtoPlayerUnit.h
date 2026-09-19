// PROTOTYPE - NOT FOR PRODUCTION
// Question: Can UE 5.6 implement Desperados III-style Showdown Mode with
//           frame-accurate command execution and a fully frozen world during planning?
// Date: 2026-09-19

#pragma once

#include "CoreMinimal.h"
#include "ProtoUnit.h"
#include "ProtoTypes.h"
#include "ProtoPlayerUnit.generated.h"

/**
 * A controllable unit with an ordered command queue.
 * Commands are queued while the world is frozen, then executed in sequence
 * once time resumes. All units begin executing on the same frame — that is
 * the simultaneity we are measuring.
 */
UCLASS()
class AProtoPlayerUnit : public AProtoUnit
{
	GENERATED_BODY()

public:
	AProtoPlayerUnit();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY() TArray<FProtoCommand> Queue;

	bool bExecuting = false;
	int32 PlayerIndex = 0;

	/** Timestamp of the most recently started command (for drift measurement). */
	float LastCommandStartTime = -1.f;
	int32 LastCommandStartSeq = -1;

	void QueueMove(const FVector& Location);
	void QueueKill(AActor* Target);
	void ClearQueue();

	void BeginExecution();

	void DrawQueue() const;
};
