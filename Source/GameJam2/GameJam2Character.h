// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameJam2Character.generated.h"

UCLASS(Blueprintable)
class AGameJam2Character : public ACharacter
{
	GENERATED_BODY()

public:
	AGameJam2Character();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//Setup A basic Bullet Projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
	UClass* BulletProjectileClass;

	//Set up a current projectile reference so that the projectile can be changed for each weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
	UClass* CurrentProjectileClass;

	//Set the firing mode (0 - Single fire, 1 - Automatic)
	void setFiringMode(int mode);

	//Placeholder Variable for ammo, more will need to be added for different weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	int CurrentHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	int MaxHealth = 100;

	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(int ammount);

	UFUNCTION(BlueprintCallable)
	void Die();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	bool bDead = false;

	//Placeholder Variable for ammo, more will need to be added for different weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
	int CurrentAmmo = 50;

	//Placeholder Variable for ammo, more will need to be added for different weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
	int CurrentMaxAmmo = 50;

	//Placeholder Variable for Clip size, more will need to be added for different weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
	int CurrentClipSize = 25;

	//Placeholder Variable for Clip size, more will need to be added for different weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
	int CurrentAmmoInClip = 25;

	//Current firing mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
	int CurrentFiringMode = 1;

	//Used in timer to determine time between bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
	float ShootSpeed = 0.2;
	bool bShootOnCooldown;
	FTimerHandle ShootSpeedTimerHandle;
	void ResetShootSpeedTimer();

	//Used in timer to determine time between bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
	float ReloadSpeed = 2;
	bool bReloading;
	FTimerHandle ReloadTimerHandle;
	void ResetReloadTimer();

	// helper variable for singe fire shooting
	bool bShootOnce = true;

	//setup sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	class USoundBase* CurrentShootSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	class USoundBase* CurrentReloadSound;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Shoot point for bullet to be spawned at */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MuzzleLocation;

	/** Flashlight */
	// COMMENTED OUT BECAUSE IT WOULDNT SETUP PARENT COMPONENT PROPERLY, NOW ADDED WITHIN BLUEPRINT
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class USpotLightComponent* Flashlight;

	//Set Up Players Input Component
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Called whenever the shoot button is clicked down */
	void ShootPressed();

	/** Called whenever the shoot button is released */
	void ShootReleased();

	//Shoot?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shooting, meta = (AllowPrivateAccess = "true"))
	bool bShoot;
};

