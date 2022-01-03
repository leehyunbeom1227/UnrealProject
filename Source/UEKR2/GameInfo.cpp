
#include "GameInfo.h"

DEFINE_LOG_CATEGORY(UEKR2);

void PrintViewport(float Time, const FColor& Color, const FString& Text)
{
	GEngine->AddOnScreenDebugMessage(-1, Time, Color, Text);
}

FString EnumToString(EPlayerJob Job)
{
	switch (Job)
	{
	case EPlayerJob::Knight:
		return TEXT("Knight");
		break;
	case EPlayerJob::Archer:
		return TEXT("Archer");
		break;
	case EPlayerJob::Magicion:
		return TEXT("Magicion");
		break;
	case EPlayerJob::Assassin:
		return TEXT("Assassin");
		break;
	}

	return TEXT("");
}

FString EnumToString(int32 Number)
{
	switch (Number)
	{
	case 0:
		return TEXT("Knight");
		break;
	case 1:
		return TEXT("Archer");
		break;
	case 2:
		return TEXT("Magicion");
		break;
	case 3:
		return TEXT("Assassin");
		break;
	}

	return TEXT("");
}

FVector FindDirVec(AActor* Actor, int32 Number)
{
	FVector Forward = Actor->GetActorForwardVector();
	FVector Right = Actor->GetActorRightVector();

	switch (Number)
	{
	case 0:
		return Forward;
		break;
	case 1:
		return (Forward + Right) / 2;
		break;
	case 2:
		return Right;
		break;
	case 3:
		return (-Forward + Right) / 2;
		break;
	case 4:
		return -Forward;
		break;
	case 5:
		return (-Forward - Right) / 2;
		break;
	case 6:
		return - Right;
		break;
	case 7:
		return (Forward - Right) / 2;
		break;
	}

	return FVector();
}

FVector FindDirVec(AActor* Actor, DirVec Vec)
{
	FVector Forward = Actor->GetActorForwardVector();
	FVector Right = Actor->GetActorRightVector();

	switch (Vec)
	{
	case DirVec::Forward:
		return Forward;
		break;	
	case DirVec::RightFor:
		return (Forward + Right) / 2;
		break;
	case DirVec::Right:
		return Right;
		break;
	case DirVec::RightBack:
		return (-Forward + Right) / 2;
		break;
	case DirVec::Back:
		return -Forward;
		break;
	case DirVec::LeftBack:
		return (-Forward - Right) / 2;
		break;
	case DirVec::Left:
		return -Right;
		break;
	case DirVec::LeftFor:
		return (Forward - Right) / 2;
		break;
	}

	return FVector();
}

EPlayerJob operator++ (EPlayerJob Job)
{
	switch (Job)
	{
	case::EPlayerJob::Knight:
		return EPlayerJob::Archer;
		break;
	case::EPlayerJob::Archer:
		return EPlayerJob::Magicion;
		break;
	case::EPlayerJob::Magicion:
		return EPlayerJob::Assassin;
		break;
	case::EPlayerJob::Assassin:
		return EPlayerJob::Knight;
		break;
	}

	return EPlayerJob::End;
}

EPlayerJob GetJob(int32 Number)
{
	switch (Number)
	{
	case 0:
		return EPlayerJob::Knight;
		break;
	case 1:
		return EPlayerJob::Archer;
		break;
	case 2:
		return EPlayerJob::Magicion;
		break;
	case 3:
		return EPlayerJob::Assassin;
		break;
	}

	return EPlayerJob::End;
}
