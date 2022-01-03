// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PrimeIdle.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAIController.h"
#include "../Player/PlayerCharacter.h"

UBTTask_PrimeIdle::UBTTask_PrimeIdle()
{
	NodeName = TEXT("PrimeIdle");
	bNotifyTick = true;

	m_AccTime = 0.f;
	m_WaitTime = 1.f;
}

EBTNodeResult::Type UBTTask_PrimeIdle::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return EBTNodeResult::Failed;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return EBTNodeResult::Failed;

	Monster->ChangeAnimType(EMonsterAnimType::Idle);
	m_AccTime = 0.f;

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_PrimeIdle::AbortTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_PrimeIdle::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// InProgress로 해두었기 때문에 여기서 안나간다.
	// 그래서 공격거리 안에 들어왔는지를 여기서도 체크하여 들어갔다면
	// Trace를 종료시킨다.
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

	m_AccTime += DeltaSeconds;

	if (m_AccTime >= m_WaitTime)
	{
		m_AccTime = 0.f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}



