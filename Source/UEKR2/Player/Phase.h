// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PlayerCharacter.h"
#include "Phase.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API APhase : public APlayerCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APhase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* m_Skill2FireMontage;

	bool        m_SkillCharging;
	bool		m_SkillFire;
	float		m_SkillCount;

public:
	bool GetSkillFire()
	{
		return m_SkillFire;
	}

	void FireEnd()
	{
		m_SkillCharging = false;
		m_SkillFire = false;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void NormalAttack();
	virtual void Attack();
	virtual void Skill2();
	void Skill2Fire();

public:
	virtual void AttackEnd();
	virtual void UseSkill(int32 Index);
	virtual void UseSkillFire(int32 Index);

};
