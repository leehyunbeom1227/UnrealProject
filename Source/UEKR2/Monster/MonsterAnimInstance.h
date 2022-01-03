// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EMonsterAnimType : uint8
{
	Spawn,
	Idle,
	Walk,
	Run,
	AttackIntro,
	Attack,
	AttackExit,
	Turn,
	Stun,
	Death,
	Skill,
	End,
};


UENUM(BlueprintType)
enum class EMonsterTurnType : uint8
{
	Left,
	Right
};

UENUM(BlueprintType)
enum class EMosnterAttackType : uint8
{
	Attack1,
	Attack2,
	Attack3,
	Attack4
};

UCLASS()
class UEKR2_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMonsterAnimInstance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool	m_OnGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EMonsterAnimType	m_AnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EMonsterAnimType	m_EndAnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EMosnterAttackType	m_AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EMonsterTurnType	m_TurnType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	m_Hit;

	bool m_UseSiegeMode;

public:
	bool GetUseSiegeMode()
	{
		return m_UseSiegeMode;
	}

	EMonsterAnimType GetAnimType()	const
	{
		return m_AnimType;
	}

	EMonsterAnimType GetEndAnimType()	const
	{
		return m_EndAnimType;
	}

	EMosnterAttackType GetAttackType()
	{
		return m_AttackType;
	}

	EMonsterTurnType GetTurnType()
	{
		return m_TurnType;
	}

	void ChangeAttackType(EMosnterAttackType Attack)
	{
		m_AttackType = Attack;
	}

	void ChangeTurnType(EMonsterTurnType Turn)
	{
		m_TurnType = Turn;
	}

	void ChangeEndAnimType(EMonsterAnimType Type)
	{
		m_EndAnimType = Type;
	}

	void ChangeAnimType(EMonsterAnimType Type)
	{
		m_AnimType = Type;
	}

	void SetUseSiegeMode(bool Use)
	{
		m_UseSiegeMode = Use;
	}

	void SetHit()
	{
		m_Hit = 1.f;
	}

	void SetAttackTypeReset()
	{
		m_AttackType = EMosnterAttackType::Attack1;
	}

public:
	void SetAttackType();

	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	UFUNCTION()
		void AnimNotify_Attack();

	UFUNCTION()
		void AnimNotify_AttackEnd();

	UFUNCTION()
		void AnimNotify_DeathEnd();

	UFUNCTION()
		void AnimNotify_HitEnd();

	UFUNCTION()
		void AnimNotify_AttackIntroEnd();

	UFUNCTION()
		void AnimNotify_AttackPosEnd();

	UFUNCTION()
		void AnimNotify_AttackExit();

	UFUNCTION()
		void AnimNotify_IdleEnd();

	UFUNCTION()
		void AnimNotify_TurnEnd();

	UFUNCTION()
		void AnimNotify_StunEnd();

	UFUNCTION()
		void AnimNotify_SpawnEnd();

	UFUNCTION()
		void AnimNotify_PrimeAttack();

	UFUNCTION()
		void AnimNotify_PrimeAttackEnd();

	UFUNCTION()
		void AnimNotify_PrimeSkillEnd();

	UFUNCTION()
		void AnimNotify_BossAttack();

	UFUNCTION()
		void AnimNotify_BossAttackEnd();

	UFUNCTION()
		void AnimNotify_BossSkillEnd();
};
