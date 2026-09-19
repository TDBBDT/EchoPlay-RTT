// PROTOTYPE - NOT FOR PRODUCTION
// Question: Can UE 5.6 implement Desperados III-style Showdown Mode with
//           frame-accurate command execution and a fully frozen world during planning?
// Date: 2026-09-19

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProtoPlayerController.generated.h"

/**
 * Handles input for the prototype.
 *
 * Uses UInputComponent::BindKey rather than action mappings or Enhanced Input:
 * raw key bindings need no .ini entries and no binary InputAction assets, which
 * keeps this prototype fully text-based and droppable into an empty project.
 */
UCLASS()
class AProtoPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProtoPlayerController();

	virtual void SetupInputComponent() override;

	/** Deproject the cursor onto the Z=0 ground plane. */
	bool TraceGroundUnderCursor(FVector& OutWorldPoint) const;

	/** Nearest live enemy within MaxDist of WorldPoint, or nullptr. */
	AActor* FindEnemyNear(const FVector& WorldPoint, float MaxDist) const;

private:
	void OnPausePressed();
	void OnExecutePressed();
	void OnSelect1Pressed();
	void OnSelect2Pressed();
	void OnToggleGatePressed();
	void OnLeftClickPressed();
};
