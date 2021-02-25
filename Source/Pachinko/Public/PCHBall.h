// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCHBall.generated.h"

class UArrowComponent;
class USphereComponent;

UCLASS()
class PACHINKO_API APCHBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCHBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision")
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
	UArrowComponent* AimingArrowComponent;

	// 1 rotates to the right, -1 rotates to the left
	int TickRotationDirectionModifier = 1;

	// 1 increases the length of the aiming arrow each tick, -1 does the opposite
	int TickArrowLengthModifier = 1;

	// 0 = Aiming, 1 = Firing, 2 = Fired
	int FiringStage = 2;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	USphereComponent* GetSphereComponent();

	UArrowComponent* GetAimingArrowComponent();

	int GetFiringStage();

	void SetFiringStage(int Val);

	void ResetAimingArrowModifiers();
};
