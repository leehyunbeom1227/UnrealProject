// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckBossEvasion.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/BossMonster.h"
#include "../Monster/MonsterAIController.h"

UBTDecorator_CheckBossEvasion::UBTDecorator_CheckBossEvasion()
{
	NodeName = TEXT("CheckBossEvasion");
}

bool UBTDecorator_CheckBossEvasion::CalculateRawConditionValue(
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

	ABossMonster* Boss = Cast<ABossMonster>(Monster);

	bool bTelePort = (Monster->GetEndAnimType() == EMonsterAnimType::Attack) &&
		(Monster->GetAnimType() == EMonsterAnimType::Attack);

	return bTelePort;
}

