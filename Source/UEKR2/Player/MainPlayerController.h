// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class UEKR2_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainPlayerController();

private:
	TArray<class AEnvironmentNormalActor*> m_CollisionEnvironmentActor;
	TArray<UUserWidget*>	m_MouseWidgetArray;
	class AMonster*			m_CursorOnMonster;

public:
	virtual void PlayerTick(float DeltaTime);
};
