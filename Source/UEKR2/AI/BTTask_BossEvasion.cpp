// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossEvasion.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/BossMonster.h"
#include "../Monster/MonsterAIController.h"

UBTTask_BossEvasion::UBTTask_BossEvasion()
{
	NodeName = TEXT("BossEvasion");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BossEvasion::ExecuteTask(
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
	{
		Monster->ChangeAnimType(EMonsterAnimType::Idle);

		Controller->StopMovement();
		return EBTNodeResult::Failed;
	}

	ABossMonster* Boss = Cast<ABossMonster>(Monster);

	FVector	MonsterLoc = Monster->GetActorLocation();
	FVector	TargetLoc = Target->GetActorLocation();

	FVector	Dir = TargetLoc - MonsterLoc;
	Dir.Normalize();

	Monster->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));

	Boss->Teleport();

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_BossEvasion::AbortTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_BossEvasion::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// InProgress로 해두었기 때문에 여기서 안나간다.
	// 그래서 공격거리 안에 들어왔는지를 여기서도 체크하여 들어갔다면
	// Trace를 종료시킨다.
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

	if (!Target)
	{
		Monster->ChangeAnimType(EMonsterAnimType::Idle);

		Controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return;
}




