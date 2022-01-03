// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"
#include "DestructibleComponent.h"
#include "Engine.h"
#include "Engine/AssetManager.h"
#include "EngineGlobals.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "MediaSoundComponent.h"
#include "MediaSource.h"
#include "Components/SplineComponent.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/WidgetTree.h"

#include "GameInfo.generated.h"


#define PACKET_SIZE 1024

/*
FString : 언리얼엔진에서 제공되는 문자열 타입이다.
FString은 앞에 * 을 붙여주면 이 FString이 실제 가지고 있는 문자열의 주소를 의미한다.

__FUNCTION__ : 현재 이 로그를 출력하는 함수 이름을 의미한다.
__LINE__ : 현재 코드의 줄번호를 의미한다.
*/

DECLARE_LOG_CATEGORY_EXTERN(UEKR2, Log, All);
#define	LOG_CALLINFO	(FString(__FUNCTION__) + TEXT("{") + FString::FromInt(__LINE__) + TEXT("}"))
#define	LOG(Format, ...)	UE_LOG(UEKR2, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define	LOGSTRING(Str)		UE_LOG(UEKR2, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *Str)

void PrintViewport(float Time, const FColor& Color, const FString& Text);


UENUM(BlueprintType)
enum class EPlayerJob : uint8
{
	Knight,
	Archer,
	Magicion,
	Assassin,
	End
};

EPlayerJob operator++ (EPlayerJob Job);

UENUM(BlueprintType)
enum class EQuestKey : uint8
{
	Quest1,
	Quest2,
	Quest3,
	Quest4,
	End
};

UENUM(BlueprintType)
enum class EMonsterKey : uint8
{
	MinionWarrior,
	MinionSiege,
	MinionGunner,
	MinionAssasion,
	MinionSuper,
	MinionPrime,
	Sevarog,
	End
};

UENUM(BlueprintType)
enum class EItemKey : uint8
{
	BFSword,
	TheBlackCleaver,
	HPPotion,
	MPPotion,
	End
};

UENUM(BlueprintType)
enum class ECheckDistanceType : uint8
{
	Trace,
	Attack
};

UENUM(BlueprintType)
enum class ETriggerType : uint8
{
	Box,
	Sphere,
	Capsule,
	Plane
};

UENUM(BlueprintType)
enum class PotionType : uint8
{
	HPPotion,
	MPPotion,
};

UENUM(BlueprintType)
enum class DirVec :	uint8
{
	Forward,
	RightFor,
	Right,
	RightBack,
	Back,
	LeftBack,
	Left,
	LeftFor
};

FString EnumToString(EPlayerJob Job);
FString EnumToString(int32 Number);
FVector FindDirVec(AActor* Actor, int32 Number);
FVector FindDirVec(AActor* Actor, DirVec Vec);
EPlayerJob	GetJob(int32 Number);

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString			Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EPlayerJob		Job;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			HPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			MPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			AttackAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float			MoveSpeed;
};

UENUM(BlueprintType)
enum class EItemOption : uint8
{
	Attack,
	Armor,
	HPMax,
	MPMax,
	HPRecovery,
	MPRecovery,
	Critical,
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon,
	Armor,
	Potion,
};

UENUM(BlueprintType)
enum class EItemOverlap : uint8
{
	None,
	Overlap,
};

USTRUCT(BlueprintType)
struct FItemOption
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EItemOption OptionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 Option;
};


USTRUCT(BlueprintType)
struct FUIItemTableInfo :
	public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString			Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EItemOverlap	Overlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EItemType		ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTexture2D*		IconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMesh*	Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString			Desc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FItemOption> OptionArray;
};

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FUIItemTableInfo Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32	Number;
};

USTRUCT()
struct FAssetPathInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Path")
		FString				Name;

	UPROPERTY(EditAnywhere, Category = "Path")
		FSoftObjectPath		Path;
};

struct RecvQueueData
{
	int32 Protocol;
	int32 Length;
	uint8 Packet[PACKET_SIZE];

	RecvQueueData() :
		Protocol(-1),
		Length(0),
		Packet{}
	{

	}
};

enum class NetworkProtocol
{
	Chat,
	UserConnect
};

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	Collection,
	Hunt,
	Communication,
	Max
};

USTRUCT(BlueprintType)
struct FQuestInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EQuestType		Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString			DestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Count;

};

UENUM(BlueprintType)
enum class EQuestCompensationType : uint8
{
	Gold,
	Item,
	Exp
};

USTRUCT(BlueprintType)
struct FQuestCompensationInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EQuestCompensationType		Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString			Compensation;
};

USTRUCT(BlueprintType)
struct FQuestTableInfo :
	public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32			Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString			Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString		QuestDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FQuestInfo>	InfoArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FQuestCompensationInfo>	CompensationArray;
};

struct FQuestDataInfo
{
	EQuestType		Type;
	FString			DestName;
	int32			Count;
	int32			MaxCount;
	bool			Complete;
};

struct FQuestData
{
	FString				Name;
	FString				QuestDesc;
	TArray<FQuestDataInfo>	CompleteArray;
	TArray<FQuestCompensationInfo>	CompensationArray;
	bool Complete;
};

enum class MonsterType
{
	MinionWarrior,
	MinionGunner,
	End
};

enum class ItemType
{
	HPRecovery,
	MPRecovery,
	End
};