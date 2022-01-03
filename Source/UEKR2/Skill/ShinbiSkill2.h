// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../Effect/DestructibleProjectile.h"
#include "ShinbiSkill2.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API AShinbiSkill2 : public ADestructibleProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShinbiSkill2();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* m_Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_StaticMesh;

	int32			m_Attack;
	class AShinbi* m_Owner;

public:
	void SetAttack(int32 Attack)
	{
		m_Attack = Attack;
	}

	void SetOwner(class AShinbi* Shinbi)
	{
		m_Owner = Shinbi;
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
