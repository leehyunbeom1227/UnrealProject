// Fill out your copyright notice in the Description page of Project Settings.


#include "MeteorTrigger.h"
#include "../Skill/Meteor.h"

// Sets default values
AMeteorTrigger::AMeteorTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMeteorTrigger::BeginPlay()
{
	Super::BeginPlay();

	m_TriggerBeginDelegate.AddDynamic(this, &AMeteorTrigger::TriggerBegin);
	m_TriggerEndDelegate.AddDynamic(this, &AMeteorTrigger::TriggerEnd);
}

// Called every frame
void AMeteorTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeteorTrigger::TriggerBegin()
{
	PrintViewport(1.f, FColor::Red, TEXT("Begin Trigger"));

	FActorSpawnParameters	param;

	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMeteor* Meteor = GetWorld()->SpawnActor<AMeteor>(AMeteor::StaticClass(),
		GetActorLocation() + FVector(0.f, 0.f, 500.f),
		FRotator::ZeroRotator, param);
}

void AMeteorTrigger::TriggerEnd()
{
	PrintViewport(1.f, FColor::Red, TEXT("End Trigger"));
}
