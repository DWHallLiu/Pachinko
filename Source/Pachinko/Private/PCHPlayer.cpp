// Fill out your copyright notice in the Description page of Project Settings.

#include "PCHPlayer.h"
#include "PCHPachinkoMachine.h"
#include "PCHBall.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APCHPlayer::APCHPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APCHPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	PachinkoMachine = Cast<APCHPachinkoMachine>(UGameplayStatics::GetActorOfClass(GetWorld(), APCHPachinkoMachine::StaticClass()));
}

// Called every frame
void APCHPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCHPlayer::AimBall()
{
	APCHBall* ActiveBall = PachinkoMachine->GetActiveBall();
	if (ActiveBall && ActiveBall->GetFiringStage() == 0) ActiveBall->SetFiringStage(1);
}

void APCHPlayer::FireBall()
{
	APCHBall* ActiveBall = PachinkoMachine->GetActiveBall();
	if (ActiveBall && ActiveBall->GetFiringStage() == 1)
	{
		ActiveBall->SetFiringStage(2);
		PachinkoMachine->FireActiveBall();
	}
	if (PachinkoMachine->GetBallResetStage() < 2 && ActiveBall->GetFiringStage() == 2) PachinkoMachine->SetBallResetStage(2);
}

// Called to bind functionality to input
void APCHPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("AimBall", IE_Pressed, this, &APCHPlayer::AimBall);
	PlayerInputComponent->BindAction("FireBall", IE_Released, this, &APCHPlayer::FireBall);
}

