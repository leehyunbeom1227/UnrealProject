// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_MonsterDetect.h"
#include "../Monster/Monster.h"
#include "../Skill/WraithSkill2.h"
#include "../Skill/PlayerSkillController.h"

UBTService_MonsterDetect::UBTService_MonsterDetect()
{
	NodeName = TEXT("MonsterDetect");
	Interval = 0.5f;
}

void UBTService_MonsterDetect::TickNode(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APlayerSkillController* Controller = Cast<APlayerSkillController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return;

	AWraithSkill2* Drone = Cast<AWraithSkill2>(Controller->GetPawn());

	if (!Drone)
		return;

	FCollisionQueryParams	params(NAME_None, false, Drone);

	FHitResult	HitResult;
	bool Sweep = GetWorld()->SweepSingleByChannel(HitResult,
		Drone->GetActorLocation(),
		Drone->GetActorLocation(), FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel11,
		FCollisionShape::MakeSphere(Drone->GetDistance()),
		params);

#if ENABLE_DRAW_DEBUG

	//FColor	DrawColor = Sweep ? FColor::Red : FColor::Green;

	//DrawDebugSphere(GetWorld(), Drone->GetActorLocation(),
	//	Drone->GetDistance(), 20, DrawColor, false,
	//	0.3f);

#endif

	if (Sweep)
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), HitResult.GetActor());

	else
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
}
