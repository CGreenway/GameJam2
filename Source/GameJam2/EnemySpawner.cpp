// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Register OnBeginOverlap function as an OnActorBeginOverlap delegate
	OnActorBeginOverlap.AddDynamic(this, &AEnemySpawner::OnBeginOverlap);

	//RoomEntrance = CreateDefaultSubobject<UMeshComponent>(TEXT("RoomEnterance"));
	//RoomEntrance->SetupAttachment(RootComponent);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootComponent);

	RoomEntrance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoomEnterance"));
	RoomEntrance->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	bStartRepeatSpawn = false;
}

void AEnemySpawner::ResetCounterTimer()
{
	SecondsAfterStart++;
	bCountTimerOnCooldown = false;
	bStartRepeatSpawn = true;
	GetWorldTimerManager().ClearTimer(CounterTimeHandle);
}

void AEnemySpawner::ResetTimeBeforeSpawnEnemy1Timer()
{
	bSpawnOnCooldown = false;
	GetWorldTimerManager().ClearTimer(TimeBeforeSpawnEnemy1Handle);
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UClass* GeneratedBPEnemy = Cast<UClass>(Enemy1);
	World->SpawnActor<AActor>(GeneratedBPEnemy, this->GetActorLocation(), this->GetActorRotation(), SpawnParams);
	bStartRepeatSpawn = true;
}

void AEnemySpawner::ResetEnemy1RepeatTimer()
{
	bSpawnOnCooldown = false;
	GetWorldTimerManager().ClearTimer(RepeatSpawnEnemy1Handle);
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UClass* GeneratedBPEnemy = Cast<UClass>(Enemy1);
	World->SpawnActor<AActor>(GeneratedBPEnemy, this->GetActorLocation(), this->GetActorRotation(), SpawnParams);
	NumberOfRepeatsEnemy1--;
	if (NumberOfRepeatsEnemy1 <= 0) {
		bRepeatSpawnEnemy1 = false;
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bSpawnEnemies && !bCountTimerOnCooldown) {
		bCountTimerOnCooldown = true;
		GetWorld()->GetTimerManager().SetTimer(CounterTimeHandle, this, &AEnemySpawner::ResetCounterTimer, 1, false);
	}
	else if (bSpawnEnemies && !bSpawnOnCooldown && bDoOnce) {
		bDoOnce = false;
		bSpawnOnCooldown = true;
		GetWorld()->GetTimerManager().SetTimer(RepeatSpawnEnemy1Handle, this, &AEnemySpawner::ResetTimeBeforeSpawnEnemy1Timer, TimeBeforeInitialSpawnEnemy1, false);
	}
	else if (bSpawnEnemies && bRepeatSpawnEnemy1 && bStartRepeatSpawn && NumberOfRepeatsEnemy1 > 0 && !bSpawnOnCooldown) {
		bSpawnOnCooldown = true;
		GetWorld()->GetTimerManager().SetTimer(RepeatSpawnEnemy1Handle, this, &AEnemySpawner::ResetEnemy1RepeatTimer, IntervalBetweenRepeatSpawnsEnemy1, false);
	}
	else if (bStartRepeatSpawn == true && bRepeatSpawnEnemy1 == false) {
		Destroy();
	}
}

void AEnemySpawner::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player") && !bSpawnEnemies) {
		bSpawnEnemies = true;
	}
}

