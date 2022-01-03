// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckPrimeAttack.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/MinionPrime.h"
#include "../Monster/MonsterAIController.h"

UBTDecorator_CheckPrimeAttack::UBTDecorator_CheckPrimeAttack()
{
	NodeName = TEXT("CheckPrimeAttack");
}

bool UBTDecorator_CheckPrimeAttack::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return false;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return false;

	bool bAttack = false;

	if (Monster->GetEndAnimType() == EMonsterAnimType::Turn && Monster->GetAnimType() == EMonsterAnimType::Turn)
		bAttack = true;

	return bAttack;
}
