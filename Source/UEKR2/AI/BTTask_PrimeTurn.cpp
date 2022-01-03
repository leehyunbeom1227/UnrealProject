// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PrimeTurn.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAIController.h"
#include "../Player/PlayerCharacter.h"

UBTTask_PrimeTurn::UBTTask_PrimeTurn()
{
	NodeName = TEXT("PrimeTurn");
	bNotifyTick = true;

	m_AccTime = 0.f;
	m_WaitTime = 1.f;
}

EBTNodeResult::Type UBTTask_PrimeTurn::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return EBTNodeResult::Failed;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return EBTNodeResult::Failed;

	APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
		return EBTNodeResult::Failed;

	FVector	MonsterLoc = Monster->GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();

	FVector	Dir = TargetLoc - MonsterLoc;
	Dir.Normalize();

	FVector PlayerLocation = Target->GetActorLocation();
	FVector PlayerVector = PlayerLocation - Monster->GetActorLocation();
	PlayerVector.Z = 0.f;
	PlayerVector.Normalize();

	FVector MonsterForwardVector = Monster->GetActorForwardVector();
	MonsterForwardVector.Z = 0.f;
	MonsterForwardVector.Normalize();

	FVector FCrossProduct = FVector::CrossProduct(MonsterForwardVector, PlayerVector);

	if (FCrossProduct.Z > 0)
		Monster->GetAnim()->ChangeTurnType(EMonsterTurnType::Left);
	else
		Monster->GetAnim()->ChangeTurnType(EMonsterTurnType::Right);

	Monster->ChangeAnimType(EMonsterAnimType::Turn);
	m_AccTime = 0.f;

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_PrimeTurn::AbortTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_PrimeTurn::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APlayerCharacter* Target = Cast<APlayerCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (Target)
	{
		m_AccTime = 0.f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	m_AccTime += DeltaSeconds;

	if (m_AccTime >= m_WaitTime)
	{
		m_AccTime = 0.f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}


