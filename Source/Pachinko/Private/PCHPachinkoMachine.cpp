// Fill out your copyright notice in the Description page of Project Settings.


#include "PCHPachinkoMachine.h"
#include "PCHBall.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
APCHPachinkoMachine::APCHPachinkoMachine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PachinkoBoardComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PachinkoBoardComponent"));
	RootComponent = PachinkoBoardComponent;

	ScoreTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreTextComponent"));
	ScoreTextComponent->SetupAttachment(RootComponent);
	BallsTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BallsTextComponent"));
	BallsTextComponent->SetupAttachment(RootComponent);

	MulliganOverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MulliganOverlapComponent"));
	MulliganOverlapComponent->SetupAttachment(RootComponent);
	MulliganOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &APCHPachinkoMachine::OnMulliganOverlapBegin);
	MulliganOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &APCHPachinkoMachine::OnMulliganOverlapEnd);

	ScoreOverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ScoreOverlapComponent"));
	ScoreOverlapComponent->SetupAttachment(RootComponent);
	ScoreOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &APCHPachinkoMachine::OnScoreOverlapBegin);
	ScoreOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &APCHPachinkoMachine::OnScoreOverlapEnd);
}

// Called when the game starts or when spawned
void APCHPachinkoMachine::BeginPlay()
{
	Super::BeginPlay();
	
	bAreColumnsFull.SetNum(14);
	for (int i = 0; i < 20; i++)
	{
		FActorSpawnParameters SpawnParams;
		Balls.Add(GetWorld()->SpawnActor<APCHBall>(Ball, FVector(-47.471931f, -32.926197f, -21.523064f) + FVector(0.f, 3.3f, 0.f) * i, FRotator::ZeroRotator, SpawnParams));
	}

	CurrentBallIndex = Balls.Num()-1;
	SetActiveBall(CurrentBallIndex);
}

// Called every frame
void APCHPachinkoMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ActiveBallLocation = ActiveBall->GetActorLocation();

	switch (BallResetStage)
	{
		case 0:
			ActiveBall->SetActorLocation(FMath::VInterpTo(ActiveBallLocation, FVector(ActiveBallLocation.X, BallLaunchLocation.Y, ActiveBallLocation.Z), DeltaTime, 10.f));
			if (ActiveBallLocation.Y == BallLaunchLocation.Y)
			{
				++BallResetStage;
				ActiveBall->SetFiringStage(0);
				ActiveBall->GetAimingArrowComponent()->SetRelativeRotation(FRotator::ZeroRotator);
				ActiveBall->GetAimingArrowComponent()->SetRelativeScale3D(FVector::OneVector);
				ActiveBall->GetAimingArrowComponent()->SetVisibility(true);
				ActiveBall->ResetAimingArrowModifiers();
			}
			break;
		case 1:
			ActiveBall->SetActorLocation(FMath::VInterpTo(ActiveBall->GetActorLocation(), BallLaunchLocation, DeltaTime, 15.f));
			if (ActiveBall->GetActorLocation() == BallLaunchLocation) ++BallResetStage;
			break;
		default: 
			break;
	}
}

APCHBall* APCHPachinkoMachine::GetActiveBall()
{
	return ActiveBall;
}

void APCHPachinkoMachine::SetActiveBall(int i)
{
	if (i >= 0)
	{
		if (i == CurrentBallIndex - 1)
		{
			for (int j = 0; j < bAreColumnsFull.Num(); j++)
			{
				if (ActiveBall->GetActorLocation().Y < -30.414146f  + 4.095439 * j)
				{
					if (!bAreColumnsFull[j])
					{
						bAreColumnsFull[j] = true;
						++Score;
					}
					break;
				}
			}
			ActiveBall = Balls[i];
			--CurrentBallIndex;
		}
		else ActiveBall = Balls[i];
		ActiveBall->GetSphereComponent()->SetGenerateOverlapEvents(false);
		ActiveBall->GetSphereComponent()->SetSimulatePhysics(false);
		ActiveBall->SetActorRotation(FRotator(30.f, 0.f, 0.f));

		BallResetStage = 0;
	}
	else for (int j = 0; j < bAreColumnsFull.Num(); j++)
	{
		if (ActiveBall->GetActorLocation().Y < -30.414146f + 4.095439 * j)
		{
			if (!bAreColumnsFull[j])
			{
				bAreColumnsFull[j] = true;
				++Score;
			}
			break;
		}
	}
	FColor ScoreColor;
	if (Score < 6 || Score > 12) ScoreColor = FColor::Cyan;
	else if (Score < 8 || Score > 10) ScoreColor = FColor::Green;
	else if (Score == 9) ScoreColor = FColor::Orange;
	else ScoreColor = FColor::Yellow;
	ScoreTextComponent->SetTextRenderColor(ScoreColor);
	BallsTextComponent->SetTextRenderColor(ScoreColor);
	if (i < 10) BallsTextComponent->SetText(FString::Printf(TEXT("0%i"), i+1)); else BallsTextComponent->SetText(FString::Printf(TEXT("%i"), i+1));
	if (Score < 10) ScoreTextComponent->SetText(FString::Printf(TEXT("0%i"), Score)); else ScoreTextComponent->SetText(FString::Printf(TEXT("%i"), Score));
}

int APCHPachinkoMachine::GetBallResetStage()
{
	return BallResetStage;
}

void APCHPachinkoMachine::SetBallResetStage(int i)
{
	BallResetStage = i;
}

void APCHPachinkoMachine::OnMulliganOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == ActiveBall)
	{
		MulliganTimerDelegate.BindUFunction(this, FName("SetActiveBall"), CurrentBallIndex); 
		GetWorldTimerManager().SetTimer(TimerHandle, MulliganTimerDelegate, 1.0f, false);
	}
}

void APCHPachinkoMachine::OnMulliganOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void APCHPachinkoMachine::OnScoreOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == ActiveBall) 
	{
		NewBallTimerDelegate.BindUFunction(this, FName("SetActiveBall"), CurrentBallIndex - 1);
		GetWorldTimerManager().SetTimer(TimerHandle, NewBallTimerDelegate, 1.0f, false);
	}
}

void APCHPachinkoMachine::OnScoreOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void APCHPachinkoMachine::FireActiveBall()
{
	if (!ActiveBall->GetSphereComponent()->IsSimulatingPhysics()) ActiveBall->GetSphereComponent()->SetSimulatePhysics(true);
	ActiveBall->GetAimingArrowComponent()->SetVisibility(false);
	ActiveBall->GetSphereComponent()->AddImpulse(ActiveBall->GetAimingArrowComponent()->GetForwardVector() * (ActiveBall->GetAimingArrowComponent()->GetRelativeScale3D().X - .5f) * 30.f);
	ActiveBall->GetSphereComponent()->SetGenerateOverlapEvents(true);
	MulliganTimerDelegate.BindUFunction(this, FName("SetActiveBall"), CurrentBallIndex);
	GetWorldTimerManager().SetTimer(TimerHandle, MulliganTimerDelegate, 1.0f, false);
}