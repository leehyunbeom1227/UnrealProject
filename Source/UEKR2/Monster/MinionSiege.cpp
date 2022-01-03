// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionSiege.h"
#include "../Effect/MinionSiegeBullet.h"
#include "MinionSiegeAIController.h"

// Sets default values
AMinionSiege::AMinionSiege()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AMinionSiegeAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Siege_Dawn.Minion_Lane_Siege_Dawn'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/Monster/BPMinionSiegeAnim.BPMinionSiegeAnim_C'"));

	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	m_MonsterInfoName = TEXT("MinionSiege");
	m_MonsterInfo.Key = (int32)EMonsterKey::MinionSiege;
}

// Called when the game starts or when spawned
void AMinionSiege::BeginPlay()
{
	Super::BeginPlay();

	m_AnimInstance->SetUseSiegeMode(true);
}

// Called every frame
void AMinionSiege::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinionSiege::NormalAttack()
{
	// Muzzle의 위치를 얻어온다.
	FVector	MuzzleLoc = GetMesh()->GetSocketLocation(TEXT("Muzzle_Front"));

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMinionSiegeBullet* Bullet = GetWorld()->SpawnActor<AMinionSiegeBullet>(
		AMinionSiegeBullet::StaticClass(), MuzzleLoc, GetActorRotation(), param);

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector Pos =  PlayerLocation - GetActorLocation();

	float PosSize = Pos.Size();

	Bullet->SetVelocity(FVector(Pos.X, Pos.Y, PosSize));
	Bullet->SetAttack(m_MonsterInfo.Attack);
	Bullet->SetOwner(this);
}