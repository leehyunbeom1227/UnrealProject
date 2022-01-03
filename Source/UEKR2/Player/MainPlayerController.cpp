// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "PlayerCharacter.h"
#include "../Monster/Monster.h"

AMainPlayerController::AMainPlayerController()
{
	bShowMouseCursor = true;
	m_CursorOnMonster = nullptr;
	//static ConstructorHelpers::FObjectFinder<UUserWidget> DefaultWidget(TEXT("WidgetBlueprint'/Game/UI/UI_MouseDefault.UI_MouseDefault'"));

	//if (DefaultWidget.Succeeded())
	//	m_MouseWidgetArray.Add(DefaultWidget.Object);

	//static ConstructorHelpers::FObjectFinder<UUserWidget> BattleWidget(TEXT("WidgetBlueprint'/Game/UI/UI_MouseIBeam.UI_MouseIBeam'"));
	//
	//if (BattleWidget.Succeeded())
	//	m_MouseWidgetArray.Add(BattleWidget.Object);
	// 
	//SetMouseCursorWidget(EMouseCursor::Default, m_MouseWidgetArray[0]);
	//SetMouseCursorWidget(EMouseCursor::Custom, m_MouseWidgetArray[1]);
}

void AMainPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	APlayerCharacter* PlayerActor = Cast<APlayerCharacter>(GetPawn());

	if (PlayerActor)
	{
		FVector	CameraLoc = PlayerActor->GetCameraLocation();
		FVector	PlayerLoc = GetPawn()->GetActorLocation();

		FCollisionQueryParams	params(NAME_None, false, this);

		TArray<FHitResult>	HitResultArray;
		bool Hit = GetWorld()->LineTraceMultiByChannel(HitResultArray, CameraLoc,
			PlayerLoc, ECollisionChannel::ECC_Camera,
			params);

		FHitResult	result;

		Hit = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9,
			false, result);


		if (Hit)
		{
			AMonster* Monster = Cast<AMonster>(result.GetActor());

			if (Monster)
			{
				CurrentMouseCursor = EMouseCursor::TextEditBeam;

				if (m_CursorOnMonster != Monster)
				{

					if (m_CursorOnMonster)
						m_CursorOnMonster->GetMesh()->SetCustomDepthStencilValue(255);

					m_CursorOnMonster = Monster;

					m_CursorOnMonster->GetMesh()->SetCustomDepthStencilValue(2);
				}
			}

			else
			{
				CurrentMouseCursor = EMouseCursor::Default;

				if (m_CursorOnMonster)
				{
					m_CursorOnMonster->GetMesh()->SetCustomDepthStencilValue(255);
					m_CursorOnMonster = nullptr;
				}
			}
		}

		else
		{
			if (m_CursorOnMonster)
			{
				m_CursorOnMonster->GetMesh()->SetCustomDepthStencilValue(255);
				m_CursorOnMonster = nullptr;
			}
		}

		//for (auto& CollisionActor : m_CollisionEnvironActor)
		//{
		//	if (!CollisionActor->GetCameraCollision())
		//	{
		//		CollisionActor->SetCameraCollision(false);
		//		m_CollisionEnvironActor.Remove(CollisionActor);
		//	}
		//}
	}
}
