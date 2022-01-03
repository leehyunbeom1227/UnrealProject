// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckPrimeIdle.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAIController.h"

UBTDecorator_CheckPrimeIdle::UBTDecorator_CheckPrimeIdle()
{
	NodeName = TEXT("CheckPrimeIdle");
}

bool UBTDecorator_CheckPrimeIdle::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return false;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return false;

	bool bIdle = false;

	if ((Monster->GetAnimType() == EMonsterAnimType::Stun || Monster->GetAnimType() == EMonsterAnimType::Skill)
		&& (Monster->GetEndAnimType() == EMonsterAnimType::Stun || Monster->GetEndAnimType() == EMonsterAnimType::Skill))
		bIdle = true;

	return bIdle;
}




