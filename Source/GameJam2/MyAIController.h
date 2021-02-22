// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM2_API AMyAIController : public AAIController
{
	GENERATED_BODY()

private:
	UBehaviorTreeComponent* BehaviorComp;
	UBlackboardComponent* BlackboardComp;

public:
	AMyAIController(); //Constructor

	//Blackboard Key
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName BlackboardKey = "Target";
	
	virtual void OnPossess(APawn* Pawn) override;

	void SetSeenTarget(APawn* Pawn);
};
