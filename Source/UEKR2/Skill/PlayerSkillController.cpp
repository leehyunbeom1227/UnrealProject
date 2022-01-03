// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkillController.h"

APlayerSkillController::APlayerSkillController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>	AITreeAsset(TEXT("BehaviorTree'/Game/Player/Wraith/BTWraithDrone.BTWraithDrone'"));

	if (AITreeAsset.Succeeded())
		m_AITree = AITreeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>	BlackboardAsset(TEXT("BlackboardData'/Game/Player/Wraith/BBWraithDrone.BBWraithDrone'"));

	if (BlackboardAsset.Succeeded())
		m_AIBlackboard = BlackboardAsset.Object;
}

void APlayerSkillController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 블랙보드를 지정한다.
	if (UseBlackboard(m_AIBlackboard, Blackboard))
	{
		// 행동트리를 동작시킨다.
		if (!RunBehaviorTree(m_AITree))
		{
			LOG(TEXT("Drone BehaviorTree Error!!"));
		}
	}
}

void APlayerSkillController::OnUnPossess()
{
	Super::OnUnPossess();
}
