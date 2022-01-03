// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DroneAttack.h"
#include "../Monster/Monster.h"
#include "../Skill/WraithSkill2.h"
#include "../Skill/PlayerSkillController.h"

UBTTask_DroneAttack::UBTTask_DroneAttack()
{
	NodeName = TEXT("DroneAttack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_DroneAttack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APlayerSkillController* Controller = Cast<APlayerSkillController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return EBTNodeResult::Failed;

	AWraithSkill2* Drone = Cast<AWraithSkill2>(Controller->GetPawn());

	if (!Drone)
		return EBTNodeResult::Failed;

	AMonster* Target = Cast<AMonster>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
		return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_DroneAttack::AbortTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_DroneAttack::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APlayerSkillController* Controller = Cast<APlayerSkillController>(OwnerComp.GetAIOwner());

	if (!Controller)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AWraithSkill2* Drone = Cast<AWraithSkill2>(Controller->GetPawn());

	if (!Drone)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AMonster* Target = Cast<AMonster>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector	DroneLoc = Drone->GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();

	float	Distance = FVector::Distance(DroneLoc, TargetLoc);

	if (Drone->GetAttackEnd())
	{
		if (Distance > Drone->GetDistance())
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		else
		{
			FVector	Dir = TargetLoc - DroneLoc;
			Dir.Normalize();

			Drone->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
			Drone->Attack(Dir);
			Drone->SetAttackEnd(false);
		}

		Drone->SetAttackEnd(false);
		return;
	}
}


