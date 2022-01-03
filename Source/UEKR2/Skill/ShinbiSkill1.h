// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../Effect/ParticleProjectile.h"
#include "ShinbiSkill1.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API AShinbiSkill1 : public AParticleProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShinbiSkill1();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* m_SecondParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* m_TrailParticle;

	int32			m_Attack;
	class AShinbi*	m_Owner;
	FVector			m_Dir;

public:
	void SetAttack(int32 Attack)
	{
		m_Attack = Attack;
	}

	void SetOwner(class AShinbi* Shinbi)
	{
		m_Owner = Shinbi;
	}

	void SetDir(FVector Dir)
	{
		m_Dir = Dir;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void StopEvent(const FHitResult& result);
};
