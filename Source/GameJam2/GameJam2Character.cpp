// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameJam2Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AGameJam2Character::AGameJam2Character()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1100.f;
	CameraBoom->SetRelativeRotation(FRotator(0.f, 0.f, -80.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a muzzle location
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(RootComponent);
	MuzzleLocation->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	// Create a Flashlight
	//Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	//Flashlight->SetupAttachment(GetCapsuleComponent());
	//Flashlight->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	//Flashlight->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AGameJam2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &AGameJam2Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGameJam2Character::MoveRight);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AGameJam2Character::ShootPressed);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AGameJam2Character::ShootReleased);
}

void AGameJam2Character::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FHitResult TraceHitResult;
		PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
		FVector CursorFV = TraceHitResult.ImpactNormal;
		FRotator CursorR = CursorFV.Rotation();
		FRotator RotationTemp = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), TraceHitResult.Location);
		float yaw = RotationTemp.Yaw;
		FRotator RotationToLookAt = FRotator(0.f, yaw, 0.f);
		this->SetActorRotation(RotationToLookAt);
	}

	if (bShoot && bDead == false) {
		if (bReloading == false && bShootOnCooldown == false && CurrentAmmo > 0 && CurrentAmmoInClip > 0) {
			if (CurrentFiringMode == 0 && bShootOnce) {
				UWorld* World = GetWorld();
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				if (CurrentProjectileClass->IsValidLowLevelFast() && MuzzleLocation->IsValidLowLevelFast()) {
					FHitResult Hit;
					/** should be more accurate but shoots into floor??/
					FRotator RotationTemp = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Hit.Location);
					MuzzleLocation->SetRelativeRotation(RotationTemp);
					/**/
					UClass* GeneratedBPBullet = Cast<UClass>(CurrentProjectileClass);
					World->SpawnActor<AActor>(GeneratedBPBullet, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation(), SpawnParams);
					if (CurrentShootSound->IsValidLowLevelFast()) {
						UGameplayStatics::PlaySoundAtLocation(this, CurrentShootSound, GetActorLocation());
					}
					bShootOnce = false;
					CurrentAmmo--;
					CurrentAmmoInClip--;
					if (CurrentAmmoInClip <= 0) {
						bReloading = true;
						GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AGameJam2Character::ResetReloadTimer, ReloadSpeed, false);
						if (CurrentReloadSound->IsValidLowLevelFast()) {
							UGameplayStatics::PlaySoundAtLocation(this, CurrentReloadSound, GetActorLocation());
						}
					}
					else {
						bShootOnCooldown = true;
						GetWorld()->GetTimerManager().SetTimer(ShootSpeedTimerHandle, this, &AGameJam2Character::ResetShootSpeedTimer, ShootSpeed, false);
					}
				}
			}
			else if (CurrentFiringMode == 1) {
				UWorld* World = GetWorld();
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				if (CurrentProjectileClass->IsValidLowLevelFast() && MuzzleLocation->IsValidLowLevelFast()) {
					FHitResult Hit;
					/** should be more accurate but shoots into floor??/
					FRotator RotationTemp = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Hit.Location);
					MuzzleLocation->SetRelativeRotation(RotationTemp);
					/**/
					UClass* GeneratedBPBullet = Cast<UClass>(CurrentProjectileClass);
					World->SpawnActor<AActor>(GeneratedBPBullet, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation(), SpawnParams);
					if (CurrentShootSound->IsValidLowLevelFast()) {
						UGameplayStatics::PlaySoundAtLocation(this, CurrentShootSound, GetActorLocation());
					}
					CurrentAmmo--;
					CurrentAmmoInClip--;
					if (CurrentAmmoInClip <= 0) {
						bReloading = true;
						GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AGameJam2Character::ResetReloadTimer, ReloadSpeed, false);
						if (CurrentReloadSound->IsValidLowLevelFast()) {
							UGameplayStatics::PlaySoundAtLocation(this, CurrentReloadSound, GetActorLocation());
						}
					}
					else {
						bShootOnCooldown = true;
						GetWorld()->GetTimerManager().SetTimer(ShootSpeedTimerHandle, this, &AGameJam2Character::ResetShootSpeedTimer, ShootSpeed, false);
					}
				}
			}
		}
	}
}

void AGameJam2Character::setFiringMode(int mode)
{
	if (mode == 0) { //Single Fire
		CurrentFiringMode = 0;
	}
	else if (mode == 1) { //Automatic
		CurrentFiringMode = 1;
	}
}

void AGameJam2Character::ReceiveDamage(int ammount)
{
	this->CurrentHealth -= ammount;
	if (CurrentHealth <= 0) {
		Die();
	}
}

void AGameJam2Character::Die()
{
	bDead = true;
}

void AGameJam2Character::ResetShootSpeedTimer()
{
	bShootOnCooldown = false;
	GetWorldTimerManager().ClearTimer(ShootSpeedTimerHandle);
}

void AGameJam2Character::ResetReloadTimer()
{
	if (CurrentAmmo >= CurrentClipSize) {
		CurrentAmmoInClip = CurrentClipSize;
	}
	else if (CurrentAmmo < CurrentClipSize && CurrentAmmo > 0) {
		CurrentAmmoInClip = CurrentAmmo;
	}
	bReloading = false;
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
}

void AGameJam2Character::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && bDead == false)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGameJam2Character::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && bDead == false)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AGameJam2Character::ShootPressed()
{
	this->bShoot = true;
	this->bShootOnce = true;
}

void AGameJam2Character::ShootReleased()
{
	this->bShoot = false;
}
