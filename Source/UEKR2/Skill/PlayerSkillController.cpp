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

	// �����带 �����Ѵ�.
	if (UseBlackboard(m_AIBlackboard, Blackboard))
	{
		// �ൿƮ���� ���۽�Ų��.
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
