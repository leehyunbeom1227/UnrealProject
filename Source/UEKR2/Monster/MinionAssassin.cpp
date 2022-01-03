// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionAssassin.h"
#include "../Effect/NormalEffect.h"
#include "MinionAIController.h"

// Sets default values
AMinionAssassin::AMinionAssassin()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AMinionAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Super_Dawn.Minion_Lane_Super_Dawn'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/Monster/BPMinionAssassinAnim.BPMinionAssassinAnim_C'"));

	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	m_MonsterInfoName = TEXT("MinionAssasion");
	m_MonsterInfo.Key = (int32)EMonsterKey::MinionAssasion;
}

// Called when the game starts or when spawned
void AMinionAssassin::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMinionAssassin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinionAssassin::NormalAttack()
{
	FVector	MinionLoc = GetActorLocation();
	FVector	Forward = GetActorForwardVector();

	FCollisionQueryParams	params(NAME_None, false, this);

	// 근접공격으로 이 타이밍에 충돌처리를 해주도록 한다.
	FHitResult	result;
	bool Sweep = GetWorld()->SweepSingleByChannel(result, MinionLoc,
		MinionLoc + Forward * m_MonsterInfo.AttackDistance, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel7,
		FCollisionShape::MakeSphere(30.f),
		params);

#if ENABLE_DRAW_DEBUG

	//FColor	DrawColor = Sweep ? FColor::Red : FColor::Green;

	//FVector Center = MinionLoc + Forward * m_MonsterInfo.AttackDistance / 2.f;

	//DrawDebugCapsule(GetWorld(), Center, m_MonsterInfo.AttackDistance / 2.f,
	//	30.f, FRotationMatrix::MakeFromZ(Forward).ToQuat(),
	//	DrawColor, false, 0.5f);

#endif

	if (Sweep)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ANormalEffect* Effect = GetWorld()->SpawnActor<ANormalEffect>(ANormalEffect::StaticClass(),
			result.ImpactPoint, result.ImpactNormal.Rotation(), param);

		Effect->LoadParticleAsync(TEXT("MinionNormal"));
		Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));


		// 데미지를 전달한다.
		FDamageEvent	DmgEvent;
		float Damage = result.GetActor()->TakeDamage(m_MonsterInfo.Attack, DmgEvent, GetController(), this);
	}
}

