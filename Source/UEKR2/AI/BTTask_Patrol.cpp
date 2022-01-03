// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAIController.h"
#include "../Player/PlayerCharacter.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = TEXT("Patrol");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return EBTNodeResult::Failed;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return EBTNodeResult::Failed;

	Monster->ChangeAnimType(EMonsterAnimType::Walk);
	Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetMonsterInfo().MoveSpeed * 0.5f;
	Monster->SetPatrolEnable(true);

	//UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, Monster->GetPatrolPoint());

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Patrol::AbortTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// InProgress�� �صξ��� ������ ���⼭ �ȳ�����.
	// �׷��� ���ݰŸ� �ȿ� ���Դ����� ���⼭�� üũ�Ͽ� ���ٸ�
	// Trace�� �����Ų��.
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
		Monster->SetPatrolEnable(false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, Monster->GetPatrolPointSpline());

	if (Monster->GetPatrolWait())
	{
		Monster->SetPatrolEnable(false);
		Monster->NextPatrolPoint();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	//// �����ߴٸ�.
	//FVector	PatrolPoint = Monster->GetPatrolPoint();
	//FVector	MonsterLoc = Monster->GetActorLocation();

	//float	CapsuleHalfHeight = Monster->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	//float	CapsuleRadius = Monster->GetCapsuleComponent()->GetUnscaledCapsuleRadius();

	//MonsterLoc.Z -= CapsuleHalfHeight;

	//float	Distance = FVector::Distance(MonsterLoc, PatrolPoint);

	//if (Distance <= CapsuleRadius)
	//{
	//	Monster->SetPatrolEnable(false);
	//	Monster->NextPatrolPoint();
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//}
}




