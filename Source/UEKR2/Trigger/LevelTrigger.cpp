// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTrigger.h"

// Sets default values
ALevelTrigger::ALevelTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

void ALevelTrigger::BeginPlay()
{
	Super::BeginPlay();

	m_TriggerBeginDelegate.AddDynamic(this, &ALevelTrigger::TriggerBegin);
	m_TriggerEndDelegate.AddDynamic(this, &ALevelTrigger::TriggerEnd);
}

// Called every frame
void ALevelTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTrigger::TriggerBegin()
{
	PrintViewport(1.f, FColor::Red, TEXT("Begin Level Trigger"));

	if (m_StreamingLevelName.Len() == 0)
		return;

	FLatentActionInfo Info;

	UGameplayStatics::LoadStreamLevel(GetWorld(), *m_StreamingLevelName, true, false, Info);
}

void ALevelTrigger::TriggerEnd()
{
	PrintViewport(1.f, FColor::Green, TEXT("End Level Trigger"));


	if (m_StreamingLevelName.Len() == 0)
		return;

	FLatentActionInfo Info;

	UGameplayStatics::UnloadStreamLevel(GetWorld(), *m_StreamingLevelName, Info, false);
}
