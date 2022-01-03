// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/WidgetComponent.h"
#include "../UI/HPBar.h"
#include "GameFramework/Character.h"
#include "Inventory.h"
#include "PlayerCharacter.generated.h"

UCLASS(config=MainAsset)
class UEKR2_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*>	m_AttackMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*>	m_SkillMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_Arm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* m_HPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneCaptureComponent2D* m_Capture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneCaptureComponent2D* m_CaptureBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* m_Trail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* m_DashMontage;

	UAnimMontage* m_FallRecoveryMontage;

	class UTextureRenderTarget2D* m_FaceTarget;

	class UTextureRenderTarget2D* m_BodyTarget;

	class UHPBar* m_HPBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FPlayerInfo	m_PlayerInfo;

	USkeletalMesh* m_PlayerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector			m_PlayerColor;

	bool    		m_OnGhostTrail;
	bool            m_ActionGhostTrail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float    		m_GhostTrailTime;
	float    		m_GhostTrailTimeAcc;

	bool			m_UseDash;
	float			m_UseDashTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			m_DashCool;

	bool			m_UseSkill1;
	float			m_UseSkill1Time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			m_Skill1Cool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			m_Skill1Power;

	bool			m_UseSkill2;
	float			m_UseSkill2Time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			m_Skill2Cool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			m_Skill2Power;

	bool			m_Change;
	float			m_ChangeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			m_ChangeCool;

	UPROPERTY(config)
		FString TestString;

	bool				m_MoveKey;
	bool				m_AttackEnable;
	bool				m_Death;

	bool			    m_BoolAttackBuff;
	float				m_AttackBuffTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			m_AttackBuffDur;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float				m_AttackBuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	m_UseSkill1MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	m_UseSkill2MP;

	class UPlayerAnim* m_AnimInstance;

	FTimerHandle m_MonsterDetectTimer;
	
	int32		m_AttackIndex;

public:
	bool GetAttackBuf()
	{
		return m_BoolAttackBuff;
	}

	float GetSkill1Attack()
	{
		return m_Skill1Power;
	}

	float GetSkill2Attack()
	{
		return m_Skill2Power;
	}

	float GetAttackBufTime()
	{
		return m_AttackBuffTime;
	}

	float GetAttackBufPower()
	{
		return m_AttackBuff;
	}

	bool GetUseDash()
	{
		return m_UseDash;
	}

	float GetUseDashTime()
	{
		return m_UseDashTime;
	}

	const FPlayerInfo& GetPlayerInfo()	const
	{
		return m_PlayerInfo;
	}

	bool IsMoveKey()	const
	{
		return m_MoveKey;
	}

	void EnableAttack(bool Enable)
	{
		m_AttackEnable = Enable;
	}

	FVector GetCameraLocation() const
	{
		return m_Camera->GetComponentLocation();
	}

	void SetAttackBuff(bool boolAttackBuf, float AttackBufTime)
	{
		m_BoolAttackBuff = boolAttackBuf;
		m_AttackBuffTime = AttackBufTime;
	}

	void SetDashTime(bool boolUseDash, float UseDashTime)
	{
		m_UseDash = boolUseDash;
		m_UseDashTime = m_UseDashTime;
	}

	void SetChangeFalse()
	{
		m_Change = false;
	}

	void AddGold(int32 Gold);
	void AddExp(int32 Exp);
	void AddHP(int32 HP);
	void AddMP(int32 MP);

	// Called when the game starts or when spawned
protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void MoveFrontKey(float Scale);
	void MoveSideKey(float Scale);
	void RotationZKey(float Scale);
	void CameraZoomKey(float Scale);
	void CameraLookUpKey(float Scale);

	void JumpKey();
	void AttackKey();
	void Skill1Key();
	void Skill2Key();
	void UsePotion1Key();
	void UsePotion2Key();
	void DashKey();
	void GameSaveKey();
	void PlayFallRecovery();

protected:
	virtual void Attack();
	virtual void Skill1();
	virtual void Skill2();
	virtual void Dash();

public:
	virtual void NormalAttack();
	virtual void AttackEnd();
	virtual void UseSkill(int32 Index);
	virtual void UseSkillFire(int32 Index);
	void GhostTrailEnd();
	void OnGhostTrail();
	void LoadParticleEffect(class ANormalEffect* Effect);
	void LoadDeathEffect();
	void ChangeCharacter();
	void UsePotion(int32 PotionNumber);

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	UFUNCTION()
		void NameWidgetCallback();

	UFUNCTION()
		void CharacterHUDNameWidgetCallback();

	UFUNCTION()
		void MonsterDetect();

public:
	void FootStep(bool Left);
};
