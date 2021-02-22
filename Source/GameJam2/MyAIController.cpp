// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "AICharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AMyAIController::AMyAIController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BlackboardComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BehaviorComp"));
}

void AMyAIController::OnPossess(APawn* pawn) {
	Super::OnPossess(pawn);

	//Get the possessed pawn if its the AI
	//Initialize blackboard

	AAICharacter* AICharacter = Cast<AAICharacter>(pawn);

	if (AICharacter) {
		if (AICharacter->BehaviorTree->BlackboardAsset) {
			BlackboardComp->InitializeBlackboard(*(AICharacter->BehaviorTree->BlackboardAsset));
			BehaviorComp->StartTree(*AICharacter->BehaviorTree);
		}
	}
}

void AMyAIController::SetSeenTarget(APawn* pawn)
{

	if (BlackboardComp) {
		BlackboardComp->SetValueAsObject(BlackboardKey, pawn);
	}
}
