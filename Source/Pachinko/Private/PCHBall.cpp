// Fill out your copyright notice in the Description page of Project Settings.

#include "PCHBall.h"
#include "PCHPachinkoMachine.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
APCHBall::APCHBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	AimingArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("AimingArrowComponent"));
	AimingArrowComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APCHBall::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation(FRotator(30.f, 0.f, 0.f));
	SphereComponent->SetGenerateOverlapEvents(false);
	AimingArrowComponent->SetHiddenInGame(false);
	AimingArrowComponent->SetVisibility(false);
}

// Called every frame
void APCHBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FiringStage == 0)
	{
		AimingArrowComponent->AddRelativeRotation(FRotator(0.f, 90.f * DeltaTime * TickRotationDirectionModifier, 0.f));
		if (FMath::Abs(AimingArrowComponent->GetRelativeRotation().Yaw) > 30.f)
		{
			AimingArrowComponent->SetRelativeRotation(FRotator(0.f, 30.f * TickRotationDirectionModifier, 0.f));
			TickRotationDirectionModifier = -TickRotationDirectionModifier;
		}
	}
	else if (FiringStage == 1)
	{
		// I wanted to try to get this to work using the "Arrow Length" variable rather than adjusting the scale to prevent the arrowhead from stretching.
		// But adjusting the variable doesn't seem to affect in-game appearance (despite adjusting it with the engine slider while running having an effect) and there is no setter.

// 		AimingArrowComponent->ArrowLength += 120.f * DeltaTime * TickArrowLengthModifier;
// 		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), AimingArrowComponent->ArrowLength));
// 		if (AimingArrowComponent->ArrowLength > 80)
// 		{
// 			AimingArrowComponent->ArrowLength = 80;
// 			TickArrowLengthModifier = -TickArrowLengthModifier;
// 		}
// 		else if (AimingArrowComponent->ArrowLength < 0)
// 		{
// 			AimingArrowComponent->ArrowLength = 0;
// 			TickArrowLengthModifier = -TickArrowLengthModifier;
// 		}

		// The below is an undesirable but acceptable solution. For now.

		AimingArrowComponent->SetRelativeScale3D(FVector(AimingArrowComponent->GetRelativeScale3D().X + 2.f * DeltaTime * TickArrowLengthModifier, 1.f, 1.f));
		if (AimingArrowComponent->GetRelativeScale3D().X > 2)
		{
			AimingArrowComponent->SetRelativeScale3D(FVector(2.f, 1.f, 1.f));
			TickArrowLengthModifier = -TickArrowLengthModifier;
		}
		else if (AimingArrowComponent->GetRelativeScale3D().X < 0.5)
		{
			AimingArrowComponent->SetRelativeScale3D(FVector(0.5f, 1.f, 1.f));
			TickArrowLengthModifier = -TickArrowLengthModifier;
		}
	}
}

USphereComponent* APCHBall::GetSphereComponent()
{
	return SphereComponent;
}

UArrowComponent* APCHBall::GetAimingArrowComponent()
{
	return AimingArrowComponent;
}

int APCHBall::GetFiringStage()
{
	return FiringStage;
}

void APCHBall::SetFiringStage(int Val)
{
	FiringStage = Val;
}

void APCHBall::ResetAimingArrowModifiers()
{
	TickRotationDirectionModifier = 1;
	TickArrowLengthModifier = 1;
}
