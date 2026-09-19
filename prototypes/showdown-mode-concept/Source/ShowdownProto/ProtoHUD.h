// PROTOTYPE - NOT FOR PRODUCTION
// Question: Can UE 5.6 implement Desperados III-style Showdown Mode with
//           frame-accurate command execution and a fully frozen world during planning?
// Date: 2026-09-19

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ProtoHUD.generated.h"

UCLASS()
class AProtoHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
};
