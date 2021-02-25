// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCHPachinkoMachine.generated.h"

class APCHBall;
class UBoxComponent;
class UTextRenderComponent;

UCLASS()
class PACHINKO_API APCHPachinkoMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCHPachinkoMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* PachinkoBoardComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Overlap")
	UBoxComponent* MulliganOverlapComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Overlap")
	UBoxComponent* ScoreOverlapComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scoreboard")
	UTextRenderComponent* ScoreTextComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scoreboard")
	UTextRenderComponent* BallsTextComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pachinko")
	TSubclassOf<APCHBall> Ball;

	TArray<APCHBall*> Balls;

	int CurrentBallIndex;

	// 0 = moving the ball on Y axis, 1 = moving the ball directly towards the ball launch location, 2 = ready to fire
	int BallResetStage {0};

	int Score {0};

	TArray<bool> bAreColumnsFull;

	FVector BallLaunchLocation {FVector(-43.658024f, 31.187786f, -17.617254f)};

	APCHBall* ActiveBall;

	FTimerHandle TimerHandle;

	FTimerDelegate MulliganTimerDelegate;

	FTimerDelegate NewBallTimerDelegate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	APCHBall* GetActiveBall();

	UFUNCTION()
	void SetActiveBall(int i);

	int GetBallResetStage();

	void SetBallResetStage(int i);

	UFUNCTION()
	void OnMulliganOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnMulliganOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnScoreOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnScoreOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void FireActiveBall();
};
