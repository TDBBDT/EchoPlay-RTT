// PROTOTYPE - NOT FOR PRODUCTION
// Question: Can UE 5.6 implement Desperados III-style Showdown Mode with
//           frame-accurate command execution and a fully frozen world during planning?
// Date: 2026-09-19

#include "ProtoPlayerUnit.h"
#include "ProtoEnemyUnit.h"
#include "DrawDebugHelpers.h"

AProtoPlayerUnit::AProtoPlayerUnit()
{
	DebugColor = FLinearColor(0.2f, 0.5f, 1.f);
}

void AProtoPlayerUnit::QueueMove(const FVector& Location)
{
	FProtoCommand Cmd;
	Cmd.Type = EProtoCommandType::Move;
	Cmd.TargetLocation = Location;
	Cmd.SequenceIndex = Queue.Num();
	Queue.Add(Cmd);
}

void AProtoPlayerUnit::QueueKill(AActor* Target)
{
	FProtoCommand Cmd;
	Cmd.Type = EProtoCommandType::Kill;
	Cmd.TargetActor = Target;
	Cmd.TargetLocation = Target ? Target->GetActorLocation() : FVector::ZeroVector;
	Cmd.SequenceIndex = Queue.Num();
	Queue.Add(Cmd);
}

void AProtoPlayerUnit::ClearQueue()
{
	Queue.Empty();
	bExecuting = false;
}

void AProtoPlayerUnit::BeginExecution()
{
	bExecuting = true;
}

void AProtoPlayerUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // base draws the body

	DrawQueue();

	if (!bExecuting || Queue.Num() == 0)
	{
		return;
	}

	FProtoCommand& Cmd = Queue[0];

	if (!Cmd.bStarted)
	{
		Cmd.bStarted = true;
		Cmd.StartTime = GetWorld()->GetTimeSeconds();
		LastCommandStartTime = Cmd.StartTime;
		LastCommandStartSeq = Cmd.SequenceIndex;
	}

	bool bDone = false;

	if (Cmd.Type == EProtoCommandType::Move)
	{
		bDone = StepToward(Cmd.TargetLocation, DeltaTime);
	}
	else // Kill
	{
		AActor* Target = Cmd.TargetActor;
		if (!Target || Target->IsActorBeingDestroyed())
		{
			bDone = true;
		}
		else if (AProtoUnit* TUnit = Cast<AProtoUnit>(Target))
		{
			if (TUnit->bDead)
			{
				bDone = true;
			}
			else if (StepToward(Target->GetActorLocation(), DeltaTime))
			{
				TUnit->bDead = true;
				TUnit->SetActorHiddenInGame(true);
				bDone = true;
			}
		}
		else
		{
			bDone = true;
		}
	}

	if (bDone)
	{
		Queue.RemoveAt(0);
		if (Queue.Num() == 0)
		{
			bExecuting = false;
		}
	}
}

void AProtoPlayerUnit::DrawQueue() const
{
	if (Queue.Num() == 0)
	{
		return;
	}

	FVector Cursor = GetActorLocation();
	for (int32 i = 0; i < Queue.Num(); ++i)
	{
		const FProtoCommand& Cmd = Queue[i];
		const bool bActive = (i == 0) && bExecuting;

		const FColor Col = (Cmd.Type == EProtoCommandType::Kill)
			? FColor(255, 80, 80)
			: (bActive ? FColor::Yellow : FColor(120, 200, 255));

		const FVector Target = Cmd.TargetLocation;

		DrawDebugLine(GetWorld(), Cursor + FVector(0.f, 0.f, 10.f),
			Target + FVector(0.f, 0.f, 10.f), Col, false, 0.f, 0, 2.f);

		DrawDebugSphere(GetWorld(), Target + FVector(0.f, 0.f, 30.f), 26.f, 12,
			Col, false, 0.f, 0, 2.f);

		Cursor = Target;
	}
}
