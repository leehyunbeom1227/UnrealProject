// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../Effect/ParticleProjectile.h"
#include "GruxSkill2.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API AGruxSkill2 : public AParticleProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGruxSkill2();
	
private:
	class AGrux* m_Owner;


public:
	void SetOwner(class AGrux* Grux)
	{
		m_Owner = Grux;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
