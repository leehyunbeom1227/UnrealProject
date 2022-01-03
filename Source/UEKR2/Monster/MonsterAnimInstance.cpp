// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "Monster.h"
#include "MinionPrime.h"
#include "BossMonster.h"
#include "../Player/MainPlayerController.h"


UMonsterAnimInstance::UMonsterAnimInstance()
{
	m_OnGround = true;

	m_AnimType = EMonsterAnimType::Idle;

	m_Hit = 0.f;

	m_EndAnimType = EMonsterAnimType::End;
}

EMosnterAttackType operator++ (EMosnterAttackType Type)
{
	switch (Type)
	{
	case::EMosnterAttackType::Attack1:
		return EMosnterAttackType::Attack2;
		break;
	case::EMosnterAttackType::Attack2:
		return EMosnterAttackType::Attack3;
		break;
	case::EMosnterAttackType::Attack3:
		return EMosnterAttackType::Attack4;
		break;
	case::EMosnterAttackType::Attack4:
		return EMosnterAttackType::Attack1;
		break;
	}

	return EMosnterAttackType::Attack1;
}

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (Monster)
	{
		UCharacterMovementComponent* Movement = Monster->GetCharacterMovement();

		if (Movement)
		{
			// ¶¥À» ¹â°í ÀÖ´ÂÁö ±¸ÇÑ´Ù.
			bool OnGround = Movement->IsMovingOnGround();

			m_OnGround = OnGround;
		}
	}
}

void UMonsterAnimInstance::AnimNotify_Attack()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (Monster)
		Monster->NormalAttack();
}

void UMonsterAnimInstance::AnimNotify_AttackEnd()
{
	//PrintViewport(1.f, FColor::Red, TEXT("AttackEnd"));
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (m_AttackType != EMosnterAttackType::Attack4)
		++m_AttackType;
	else
		m_AttackType = EMosnterAttackType::Attack1;

	if (Monster)
		Monster->SetAttackEnd(true);
}

void UMonsterAnimInstance::AnimNotify_DeathEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (Monster)
		Monster->Death();
}

void UMonsterAnimInstance::AnimNotify_HitEnd()
{
	m_Hit = 0.f;
}

void UMonsterAnimInstance::AnimNotify_AttackIntroEnd()
{
	ChangeAnimType(EMonsterAnimType::Attack);
}

void UMonsterAnimInstance::AnimNotify_AttackPosEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector Pos = PlayerLocation - Monster->GetActorLocation();

	if (Monster->GetMonsterInfo().AttackDistance >= Pos.GetAbs().Size())
		ChangeAnimType(EMonsterAnimType::AttackExit);
}

void UMonsterAnimInstance::AnimNotify_AttackExit()
{
	ChangeAnimType(EMonsterAnimType::Idle);
}

void UMonsterAnimInstance::AnimNotify_SpawnEnd()
{
	ChangeAnimType(EMonsterAnimType::Idle);
}

void UMonsterAnimInstance::AnimNotify_IdleEnd()
{
	m_EndAnimType = EMonsterAnimType::Idle;
}

void UMonsterAnimInstance::AnimNotify_TurnEnd()
{
	m_EndAnimType = EMonsterAnimType::Turn;
}

void UMonsterAnimInstance::AnimNotify_StunEnd()
{
	m_EndAnimType = EMonsterAnimType::Stun;
}

void UMonsterAnimInstance::AnimNotify_PrimeAttack()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	AMinionPrime* Prime = Cast<AMinionPrime>(Monster);

	Prime->SpawnPrimeAttack();
}

void UMonsterAnimInstance::AnimNotify_PrimeAttackEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (Monster)
		Monster->SetAttackEnd(true);

	m_EndAnimType = EMonsterAnimType::Attack;
}

void UMonsterAnimInstance::AnimNotify_PrimeSkillEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (Monster)
		Monster->SetAttackEnd(true);

	m_EndAnimType = EMonsterAnimType::Skill;
}

void UMonsterAnimInstance::AnimNotify_BossAttack()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	ABossMonster* Boss = Cast<ABossMonster>(Monster);

	Boss->SpawnAttack();
}

void UMonsterAnimInstance::AnimNotify_BossAttackEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (Monster)
		Monster->SetAttackEnd(true);

	ABossMonster* Boss = Cast<ABossMonster>(Monster);

	Boss->SetCanTeleport();

	m_EndAnimType = EMonsterAnimType::Attack;
}

void UMonsterAnimInstance::AnimNotify_BossSkillEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (Monster)
		Monster->SetAttackEnd(true);

	ABossMonster* Boss = Cast<ABossMonster>(Monster);

	Boss->SetCanTeleport();

	m_EndAnimType = EMonsterAnimType::Skill;
}

void UMonsterAnimInstance::SetAttackType()
{
	int iRan = rand() % 4;

	switch (iRan)
	{
	case 0:
		m_AttackType = EMosnterAttackType::Attack1;
		break;
	case 1:
		m_AttackType = EMosnterAttackType::Attack2;
		break;
	case 2:
		m_AttackType = EMosnterAttackType::Attack3;
		break;
	case 3:
		m_AttackType = EMosnterAttackType::Attack4;
		break;
	}
}

