// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../UI/BossHPBar.h"
#include "Minion.h"
#include "MinionPrime.generated.h"

UCLASS()
class UEKR2_API AMinionPrime : public AMinion
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMinionPrime();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBossHPBar* m_BossHPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	m_WaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class APortalTrigger* m_Portal;

	float	m_AccTime;

	float	m_TurnFloat;

	bool	m_CanAttack;

public:
	bool CanAttack()
	{
		return m_CanAttack;
	}

public:
	void SpawnPrimeAttack();
	
public:
	virtual void Death();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

};
