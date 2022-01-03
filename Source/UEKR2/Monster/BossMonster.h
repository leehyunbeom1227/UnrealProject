// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../UI/BossHPBar.h"
#include "Monster.h"
#include "BossMonster.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API ABossMonster : public AMonster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossMonster();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBossHPBar* m_BossHPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class APortalTrigger* m_Portal;

	bool	m_UseTeleport;
	bool	m_Teleport;

	bool	m_CanAttack;

	FVector m_FirstLoc;

public:
	bool IsUseTeleport()
	{
		return m_UseTeleport;
	}

	bool CanTeleport()
	{
		return m_Teleport;
	}

	bool CanAttack()
	{
		return m_CanAttack;
	}

	void SetCanTeleport()
	{
		m_Teleport = true;
	}

	void SetFalseUseTeleport()
	{
		m_UseTeleport = false;
	}

public:
	void SpawnAttack();
	void Teleport();
	virtual void Death();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

};
