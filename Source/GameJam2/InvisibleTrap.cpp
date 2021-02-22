// Fill out your copyright notice in the Description page of Project Settings.


#include "InvisibleTrap.h"
#include "GameJam2Character.h"

// Sets default values
AInvisibleTrap::AInvisibleTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TrapBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trap Base"));
	TrapBase->SetupAttachment(RootComponent);

	TrapAnimatedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trap Animated Mesh"));
	TrapAnimatedMesh->SetupAttachment(TrapBase);
	TrapAnimatedMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	CollisionMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision Mesh Comp"));
	CollisionMeshComponent->SetupAttachment(TrapBase);
	CollisionMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	//Register OnBeginOverlap function as an OnActorBeginOverlap delegate
	OnActorBeginOverlap.AddDynamic(this, &AInvisibleTrap::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AInvisibleTrap::BeginPlay()
{
	Super::BeginPlay();
	TrapBase->SetMaterial(0, Semi);
	TrapAnimatedMesh->SetMaterial(0, Invisible);
}

// Called every frame
void AInvisibleTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInvisibleTrap::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player")) {
		TrapBase->SetMaterial(0, Visible);
		TrapAnimatedMesh->SetMaterial(0, Visible);
		AGameJam2Character* player = Cast<AGameJam2Character>(OtherActor);
		player->ReceiveDamage(this->DamageGiven);
	}
}

