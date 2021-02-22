// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "MyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"


// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Initialise pawn sensing component

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	//SetPeripheral vision to 90 **WILL MOST LIKELY CHANGE FOR CONE VISION
	PawnSensingComp->SetPeripheralVisionAngle(90.f);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(RootComponent);
	MuzzleLocation->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	//Register function that is going to fire when character sees pawn

	if (PawnSensingComp) {
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AAICharacter::OnSeePlayer);
	}
	
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Fired");
	Fire();
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAICharacter::OnSeePlayer(APawn *pawn)
{
	AMyAIController* AIController = Cast<AMyAIController>(GetController());

	if (AIController) {
		GLog->Log("Hello There");
	}

	AIController->SetSeenTarget(pawn);
}


void AAICharacter::Fire() {
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UClass* GeneratedBPBullet = Cast<UClass>(CurrentProjectileClass);
	World->SpawnActor<AActor>(GeneratedBPBullet, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation(), SpawnParams);

	
}

