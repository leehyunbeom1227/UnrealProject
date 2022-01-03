// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "WraithSkill2.generated.h"

UCLASS()
class UEKR2_API AWraithSkill2 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWraithSkill2();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* m_Body;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* m_Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* m_Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	m_DestroyTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	m_Distance;

	float	m_DestroyTime;

	int32			m_Attack;
	class AWraith*  m_Owner;
	bool			m_Wait;
	float			m_followAccTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float			m_followTime;

	float			m_AttackAccTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			m_AttackTime;

	bool			m_AttackEnd;

public:
	bool GetAttackEnd()
	{
		return m_AttackEnd;
	}

	float GetDistance()
	{
		return m_Distance;
	}

	void SetAttackEnd(bool Bool)
	{
		m_AttackEnd = Bool;
	}

	void SetAttack(int32 Attack)
	{
		m_Attack = Attack;
	}

	void SetOwner(class AWraith* Wraith)
	{
		m_Owner = Wraith;
	}

public:
	void Attack(FVector Dir);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
