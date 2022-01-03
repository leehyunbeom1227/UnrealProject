// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckPrimeTurn.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/MinionPrime.h"
#include "../Monster/MonsterAnimInstance.h"
#include "../Monster/MonsterAIController.h"

UBTDecorator_CheckPrimeTurn::UBTDecorator_CheckPrimeTurn()
{
	NodeName = TEXT("CheckPrimeTurn");
}

bool UBTDecorator_CheckPrimeTurn::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return false;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return false;

	bool bTurn = false;

	if (Monster->GetEndAnimType() == EMonsterAnimType::Idle && Monster->GetAnimType() == EMonsterAnimType::Idle)
		bTurn = true;

	AMinionPrime* Prime = Cast<AMinionPrime>(Monster);
	Prime->GetAnim()->SetAttackType();

	if (Prime->GetAnim()->GetAttackType() != EMosnterAttackType::Attack4)
		Controller->GetBlackboardComponent()->SetValueAsBool(TEXT("AttackDis"), true);

	else
		Controller->GetBlackboardComponent()->SetValueAsBool(TEXT("AttackDis"), false);

	return bTurn;
}

