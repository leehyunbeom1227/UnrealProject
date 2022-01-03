// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckBossUltimate.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/BossMonster.h"
#include "../Monster/MonsterAIController.h"

UBTDecorator_CheckBossUltimate::UBTDecorator_CheckBossUltimate()
{
	NodeName = TEXT("CheckBossUltimate");
}

bool UBTDecorator_CheckBossUltimate::CalculateRawConditionValue(
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

	if (Monster->GetEndAnimType() == EMonsterAnimType::Idle &&
		Monster->GetAnimType() == EMonsterAnimType::Idle)
		bAttack = true;

	return bAttack;
}

