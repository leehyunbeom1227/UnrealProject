// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DroneWait.h"
#include "../Monster/Monster.h"
#include "../Skill/WraithSkill2.h"
#include "../Skill/PlayerSkillController.h"

UBTTask_DroneWait::UBTTask_DroneWait()
{
	NodeName = TEXT("DroneWait");
	bNotifyTick = true;

}

EBTNodeResult::Type UBTTask_DroneWait::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APlayerSkillController* Controller = Cast<APlayerSkillController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return EBTNodeResult::Failed;

	AWraithSkill2* Drone = Cast<AWraithSkill2>(Controller->GetPawn());

	if (!Drone)
		return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_DroneWait::AbortTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_DroneWait::TickTask(UBehaviorTreeComponent& OwnerComp,
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

	if (Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return;
}



