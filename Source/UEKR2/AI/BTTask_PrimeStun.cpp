// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PrimeStun.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAIController.h"
#include "../Player/PlayerCharacter.h"

UBTTask_PrimeStun::UBTTask_PrimeStun()
{
	NodeName = TEXT("PrimeTurn");
	bNotifyTick = true;

	m_AccTime = 0.f;
	m_WaitTime = 1.f;
}

EBTNodeResult::Type UBTTask_PrimeStun::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return EBTNodeResult::Failed;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return EBTNodeResult::Failed;

	Monster->ChangeAnimType(EMonsterAnimType::Stun);

	Controller->GetBlackboardComponent()->SetValueAsBool(TEXT("AttackDis"), false);

	m_AccTime = 0.f;

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_PrimeStun::AbortTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_PrimeStun::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (Target)
	{
		m_AccTime = 0.f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	m_AccTime += DeltaSeconds;

	if (m_AccTime >= m_WaitTime)
	{
		m_AccTime = 0.f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}




