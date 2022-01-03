// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckPrimeStun.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAIController.h"

UBTDecorator_CheckPrimeStun::UBTDecorator_CheckPrimeStun()
{
	NodeName = TEXT("CheckPrimeStun");
}

bool UBTDecorator_CheckPrimeStun::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return false;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return false;

	bool bStun = false;

	if (Monster->GetEndAnimType() == EMonsterAnimType::Attack && Monster->GetAnimType() == EMonsterAnimType::Attack)
		bStun = true;

	return bStun;
}




