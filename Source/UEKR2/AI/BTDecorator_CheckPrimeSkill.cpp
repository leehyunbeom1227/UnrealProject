// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckPrimeSkill.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAIController.h"

UBTDecorator_CheckPrimeSkill::UBTDecorator_CheckPrimeSkill()
{
	NodeName = TEXT("CheckPrimeSkill");
}

bool UBTDecorator_CheckPrimeSkill::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return false;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return false;

	bool bSKill = false;

	if (Monster->GetEndAnimType() == EMonsterAnimType::Turn && Monster->GetAnimType() == EMonsterAnimType::Turn)
		bSKill = true;

	return bSKill;
}



