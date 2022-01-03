// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BoxTrigger.h"
#include "PortalOpenTrigger.generated.h"

UCLASS()
class UEKR2_API APortalOpenTrigger : public ABoxTrigger
{
	GENERATED_BODY()

public:
	APortalOpenTrigger();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ULevelSequence* m_SequenceAsset;

	ULevelSequencePlayer* m_SequencePlayer;

	ALevelSequenceActor* m_SequenceActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* m_ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class APortalTrigger* m_Portal;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void TriggerBegin();

};
