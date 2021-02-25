// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PCHPlayer.generated.h"

class APCHPachinkoMachine;

UCLASS()
class PACHINKO_API APCHPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APCHPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	APCHPachinkoMachine* PachinkoMachine;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AimBall();

	void FireBall();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
