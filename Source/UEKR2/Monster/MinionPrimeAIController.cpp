// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionPrimeAIController.h"

AMinionPrimeAIController::AMinionPrimeAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>	AITreeAsset(TEXT("BehaviorTree'/Game/Monster/BTMinionPrime.BTMinionPrime'"));

	if (AITreeAsset.Succeeded())
		m_AITree = AITreeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>	BlackboardAsset(TEXT("BlackboardData'/Game/Monster/BBMonster.BBMonster'"));

	if (BlackboardAsset.Succeeded())
		m_AIBlackboard = BlackboardAsset.Object;
}

void AMinionPrimeAIController::OnPossess(APawn* InPawn)
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

void AMinionPrimeAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
