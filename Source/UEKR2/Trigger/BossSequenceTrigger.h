// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BoxTrigger.h"
#include "BossSequenceTrigger.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API ABossSequenceTrigger : public ABoxTrigger
{
	GENERATED_BODY()

public:
	ABossSequenceTrigger();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ULevelSequence* m_SequenceAsset;

	ULevelSequencePlayer* m_SequencePlayer;

	ALevelSequenceActor* m_SequenceActor;

public:
	UFUNCTION()
		void TriggerBegin();

	UFUNCTION()
		void TriggerEnd();
};
