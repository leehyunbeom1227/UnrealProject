// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckMonsterHP.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAIController.h"

UBTDecorator_CheckMonsterHP::UBTDecorator_CheckMonsterHP()
{
	NodeName = TEXT("CheckMonsterHP");
}

bool UBTDecorator_CheckMonsterHP::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return false;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return false;

	APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
		return false;

	const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();

	float HPPercent = (float)MonsterInfo.HP / (float)MonsterInfo.HPMax;

	return HPPercent <= m_CheckHPPercent;
}


