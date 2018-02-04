// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "PlayerCamera.generated.h"

UCLASS()
class MICE_API APlayerCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCamera();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* Camera;
public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void MoveForward(float Val);
	
	UFUNCTION()
		void MoveRightward(float Val);
};
