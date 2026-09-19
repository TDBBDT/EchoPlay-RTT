// PROTOTYPE - NOT FOR PRODUCTION
// Question: Can UE 5.6 implement Desperados III-style Showdown Mode with
//           frame-accurate command execution and a fully frozen world during planning?
// Date: 2026-09-19

#pragma once

#include "CoreMinimal.h"
#include "ProtoTypes.generated.h"

UENUM()
enum class EProtoCommandType : uint8
{
	Move,
	Kill
};

USTRUCT()
struct FProtoCommand
{
	GENERATED_BODY()

	UPROPERTY() EProtoCommandType Type = EProtoCommandType::Move;
	UPROPERTY() FVector TargetLocation = FVector::ZeroVector;
	UPROPERTY() TObjectPtr<AActor> TargetActor = nullptr;

	// Runtime execution state (not serialized — prototype only)
	UPROPERTY() bool bStarted = false;
	UPROPERTY() float StartTime = -1.f;
	UPROPERTY() int32 SequenceIndex = 0;
};
