// Fill out your copyright notice in the Description page of Project Settings.


#include "Minion.h"
#include "MinionAIController.h"

// Sets default values
AMinion::AMinion()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AMinionAIController::StaticClass();

}

// Called when the game starts or when spawned
void AMinion::BeginPlay()
{
	GetMesh()->CreateDynamicMaterialInstance(0);

	GetMesh()->bRenderCustomDepth = true;
	GetMesh()->CustomDepthStencilWriteMask = ERendererStencilMask::ERSM_Default;

	Super::BeginPlay();
}

// Called every frame
void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
