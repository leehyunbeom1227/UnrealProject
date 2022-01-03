// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerDetect.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAIController.h"

UBTService_PlayerDetect::UBTService_PlayerDetect()
{
	NodeName = TEXT("PlayerDetect");
	Interval = 0.5f;
}

void UBTService_PlayerDetect::TickNode(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, 
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return;

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return;

	const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();

	// 주변에 플레이어가 있는지 판단한다.
	FCollisionQueryParams	params(NAME_None, false, Monster);

	// 근접공격으로 이 타이밍에 충돌처리를 해주도록 한다.
	FHitResult	HitResult;
	bool Hit = false;

	bool SweepTrace = GetWorld()->SweepSingleByChannel(HitResult,
		Monster->GetActorLocation(), Monster->GetActorLocation(),
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel6, 
		FCollisionShape::MakeSphere(Monster->GetMonsterInfo().TraceDistance),
		params);

	bool SweepCircle = GetWorld()->SweepSingleByChannel(HitResult,
		Monster->GetActorLocation(), Monster->GetActorLocation(),
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(Monster->GetMonsterInfo().TraceDistance * 0.5),
		params);

	if (SweepTrace)
	{
		FVector	Dir = HitResult.ImpactPoint - Monster->GetActorLocation();

		Dir.Normalize();

		FVector Forward = Monster->GetActorForwardVector();

		Forward.Normalize();

		float Dot = FVector::DotProduct(Dir, Forward);
		float Angle = FMath::Acos(Dot);
		Angle = FMath::RadiansToDegrees(Angle);

		if (Angle <= Monster->GetMonsterInfo().AttackAngle)
			Hit = true;
	}

	if (SweepCircle)
		Hit = true;

#if ENABLE_DRAW_DEBUG

	//FColor	DrawColor = Hit ? FColor::Red : FColor::Green;
	//
	//DrawDebugCone(GetWorld(), Monster->GetActorLocation(),
	//	Monster->GetActorForwardVector(),
	//	MonsterInfo.TraceDistance,
	//	FMath::DegreesToRadians(MonsterInfo.AttackAngle),
	//	FMath::DegreesToRadians(MonsterInfo.AttackAngle),
	//	20,	DrawColor, false, 0.3f);

	//DrawDebugSphere(GetWorld(), Monster->GetActorLocation(),
	//	MonsterInfo.TraceDistance * 0.5, 20, DrawColor, false, 0.3f);

#endif

	if (Hit)
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), HitResult.GetActor());

	else
	{
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
		Monster->GetAnim()->SetAttackTypeReset();
	}
}
