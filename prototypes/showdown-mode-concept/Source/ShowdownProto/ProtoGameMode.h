// PROTOTYPE - NOT FOR PRODUCTION
// Question: Can UE 5.6 implement Desperados III-style Showdown Mode with
//           frame-accurate command execution and a fully frozen world during planning?
// Date: 2026-09-19

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProtoGameMode.generated.h"

class AProtoPlayerUnit;
class AProtoEnemyUnit;

/**
 * Builds the entire prototype world at BeginPlay, so the level can be completely
 * empty — no actors to place, no blueprints to wire.
 */
UCLASS()
class AProtoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProtoGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY() TArray<AProtoPlayerUnit*> PlayerUnits;
	UPROPERTY() TArray<AProtoEnemyUnit*> EnemyUnits;

	bool bIsPaused = false;
	int32 SelectedIndex = 0;

	/** Debug toggle (V): whether the explicit pause gate is applied to enemy vision. */
	bool bPauseGateEnabled = true;

	/** Last action feedback shown on the HUD. */
	FString StatusMessage;

	int32 ExecuteCount = 0;

	// --- Actions ---
	void SetPaused(bool bPause);
	void ExecuteAll();
	void SelectUnit(int32 Index);
	void TogglePauseGate();
	void UnpauseAndDiscard();

	AProtoPlayerUnit* GetSelected() const;
	void QueueCommandForSelected(const FVector& WorldLocation, AActor* TargetActor);

	/** |t0 - t1| in ms between the two units' most recently started commands. */
	float ComputeSimultaneityDriftMs() const;

	/** Total number of times an enemy detected a player while the world was frozen. */
	int32 GetDetectionsWhilePaused() const;

	static AProtoGameMode* Get(const UObject* WorldContext);

private:
	void BuildWorld();
	void ApplyTimeStateToUnits();
	void DrawGroundGrid() const;
};
