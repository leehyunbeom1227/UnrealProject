// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger.h"

// Sets default values
ATrigger::ATrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrigger::BeginPlay()
{
	Super::BeginPlay();
	
	m_Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATrigger::TriggerBeginOverlap);
	m_Trigger->OnComponentEndOverlap.AddDynamic(this, &ATrigger::TriggerEndOverlap);

}

// Called every frame
void ATrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrigger::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep
	, const FHitResult& SweepResult)
{
	m_TriggerBeginDelegate.Broadcast();
}


void ATrigger::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	m_TriggerEndDelegate.Broadcast();
}