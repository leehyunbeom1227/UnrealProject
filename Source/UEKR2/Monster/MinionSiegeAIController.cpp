// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionSiegeAIController.h"


AMinionSiegeAIController::AMinionSiegeAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>	AITreeAsset(TEXT("BehaviorTree'/Game/Monster/BTMinionSiege.BTMinionSiege'"));

	if (AITreeAsset.Succeeded())
		m_AITree = AITreeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>	BlackboardAsset(TEXT("BlackboardData'/Game/Monster/BBMonster.BBMonster'"));

	if (BlackboardAsset.Succeeded())
		m_AIBlackboard = BlackboardAsset.Object;
}

void AMinionSiegeAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(m_AIBlackboard, Blackboard))
	{
		if (!RunBehaviorTree(m_AITree))
		{
			LOG(TEXT("Monster BehaviorTree Error"));
		}
	}
}

void AMinionSiegeAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
