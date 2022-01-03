// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BoxTrigger.h"
#include "LevelTrigger.generated.h"

/**
 * 
 */
UCLASS()
class UEKR2_API ALevelTrigger : public ABoxTrigger
{
	GENERATED_BODY()

public:
	ALevelTrigger();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString m_StreamingLevelName;

public:
	UFUNCTION()
		void TriggerBegin();

	UFUNCTION()
		void TriggerEnd();
	
};
