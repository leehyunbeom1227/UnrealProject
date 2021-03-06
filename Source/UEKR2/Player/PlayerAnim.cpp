// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"

UPlayerAnim::UPlayerAnim()
{
	m_Direction = 0.f;
	m_Speed = 0.f;
	m_Attack = false;
	m_OnGround = true;
	m_Dead = false;
	m_FallRecoveryAdditive = 0.f;

	m_AnimType = EPlayerAnimType::Ground;
}

void UPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		UCharacterMovementComponent* Movement = Player->GetCharacterMovement();

		if (Movement)
		{
			// 속도를 구한다.
			m_Speed = Movement->Velocity.Size();

			// 땅을 밟고 있는지 구한다.
			bool OnGround = Movement->IsMovingOnGround();

			//if(!m_OnGround && OnGround && Player->IsMoveKey())
			//	m_AnimType = EPlayerAnimType::Ground;
		
			m_OnGround = OnGround;

			if (!m_OnGround && m_AnimType != EPlayerAnimType::Jump && m_AnimType != EPlayerAnimType::Dash)
			{
				m_AnimType = EPlayerAnimType::Fall;
				m_FallRecoveryAdditive = 0.f;
			}

		}
	}
}

void UPlayerAnim::AnimNotify_AttackEnd()
{
	m_Attack = false;
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->AttackEnd();
}

void UPlayerAnim::AnimNotify_NormalAttack()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->NormalAttack();
}

void UPlayerAnim::AnimNotify_AttackCombo()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->EnableAttack(true);
}

void UPlayerAnim::AnimNotify_JumpEnd()
{
	m_AnimType = EPlayerAnimType::Fall;
}

void UPlayerAnim::AnimNotify_LeftStep()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->FootStep(true);
}

void UPlayerAnim::AnimNotify_RightStep()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->FootStep(false);
}

void UPlayerAnim::AnimNotify_FallStart()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->FootStep(false);
}

void UPlayerAnim::AnimNotify_FallEnd()
{
	m_AnimType = EPlayerAnimType::Ground;

	m_FallRecoveryAdditive = 1;

	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->PlayFallRecovery();
}

void UPlayerAnim::AnimNotify_FallRecoveryEnd()
{
	m_FallRecoveryAdditive = 0.f;
}

void UPlayerAnim::AnimNotify_SkillFire()
{
	m_Attack = false;
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		Player->UseSkill(0);
		Player->AttackEnd();
	}
}
void UPlayerAnim::AnimNotify_OnMagicCircle()
{
	m_Attack = false;
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		Player->UseSkill(1);
		Player->AttackEnd();
	}
}

void UPlayerAnim::AnimNotify_Skill2Fire()
{
	m_Attack = false;
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		Player->UseSkillFire(1);
		Player->AttackEnd();
	}
}

void UPlayerAnim::AnimNotify_GhostTrailEnd()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->GhostTrailEnd();
}

void UPlayerAnim::AnimNotify_DashEnd()
{
	m_AnimType = EPlayerAnimType::Fall;

	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
		Player->GhostTrailEnd();
}

void UPlayerAnim::AnimNotify_UseSkill1()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		Player->UseSkill(0);
		Player->AttackEnd();
	}
}

void UPlayerAnim::AnimNotify_UseSkill2()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		Player->UseSkill(1);
		Player->AttackEnd();
	}
}

void UPlayerAnim::AnimNotify_SkillEnd()
{
	m_Skill = false;
}

void UPlayerAnim::AnimNotify_DeadEnd()
{
	m_Dead = true;
}
