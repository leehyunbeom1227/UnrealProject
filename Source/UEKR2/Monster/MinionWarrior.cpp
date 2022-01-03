// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionWarrior.h"
#include "../Effect/NormalEffect.h"
#include "MinionAIController.h"

// Sets default values
AMinionWarrior::AMinionWarrior()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AMinionAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Melee_Dawn.Minion_Lane_Melee_Dawn'"));

	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/Monster/BPMinionWarriorAnim.BPMinionWarriorAnim_C'"));

	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	m_MonsterInfoName = TEXT("MinionWarrior");

	m_MonsterInfo.Key = (int32)EMonsterKey::MinionWarrior;
}

// Called when the game starts or when spawned
void AMinionWarrior::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMinionWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinionWarrior::NormalAttack()
{
	FVector	MinionLoc = GetActorLocation();
	FVector	Forward = GetActorForwardVector();

	FCollisionQueryParams	params(NAME_None, false, this);

	// ������������ �� Ÿ�ֿ̹� �浹ó���� ���ֵ��� �Ѵ�.
	FHitResult	result;
	bool Sweep = GetWorld()->SweepSingleByChannel(result, MinionLoc,
		MinionLoc + Forward * m_MonsterInfo.AttackDistance, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel7, 
		FCollisionShape::MakeSphere(30.f),
		params);

#if ENABLE_DRAW_DEBUG

	//FColor	DrawColor = Sweep ? FColor::Red : FColor::Green;

	//FVector Center = MinionLoc + Forward * m_MonsterInfo.AttackDistance / 2.f;
	//
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

		// Sound
		Effect->LoadSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));


		// �������� �����Ѵ�.
		FDamageEvent	DmgEvent;
		float Damage = result.GetActor()->TakeDamage(m_MonsterInfo.Attack, DmgEvent, GetController(), this);
	}
}

