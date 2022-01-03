// Fill out your copyright notice in the Description page of Project Settings.


#include "StartWidget.h"

void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));
	m_QuitButton = Cast<UButton>(GetWidgetFromName(TEXT("QuitButton")));

	m_StartButton->OnClicked.AddDynamic(this, &UStartWidget::StartButton);
	m_QuitButton->OnClicked.AddDynamic(this, &UStartWidget::QuitButton);

	m_StartButton->OnHovered.AddDynamic(this, &UStartWidget::StartButtonHOvered);
	m_StartButton->OnHovered.AddDynamic(this, &UStartWidget::QuitButtonHOvered);

	// 애니메이션을 찾는다.
	BindingAnimations();
}

void UStartWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UStartWidget::BindingAnimations()
{
	UProperty* Prop = GetClass()->PropertyLink;

	while (Prop != nullptr)
	{
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);

			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);

				UWidgetAnimation* Anim = Cast<UWidgetAnimation>(Obj);

				if (Anim != nullptr)
				{
					if (Anim->MovieScene->GetFName() == "StartButtonMouseOn")
					{
						m_StartButtonMouseOn = Anim;
						LOG(TEXT("StartButtonAnim"));
					}
				}
			}
		}

		Prop = Prop->PropertyLinkNext;
	}
}

void UStartWidget::StartButton()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("ElvenRuins"));
}

void UStartWidget::QuitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),
		EQuitPreference::Quit, true);
}

void UStartWidget::StartButtonHOvered()
{
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main"));
	PlayAnimation(m_StartButtonMouseOn);
	LOG(TEXT("StartButtonAnim MouseOn"));
}

void UStartWidget::QuitButtonHOvered()
{
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),
	//	EQuitPreference::Quit, true);
}
