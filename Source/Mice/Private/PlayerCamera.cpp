// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCamera.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "../Public/PlayerCamera.h"


// Sets default values
APlayerCamera::APlayerCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	speed = 10;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(root);
	Camera->SetRelativeTransform(FTransform(FRotator(-45.0f, -45.0f, 0.0f)));
}

// Called when the game starts or when spawned
void APlayerCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &APlayerCamera::MoveForward);
	PlayerInputComponent->BindAxis("Rightward", this, &APlayerCamera::MoveRightward);
}

void APlayerCamera::MoveForward(float Val)
{
	FVector vec = FVector(speed * Val, 0.0f, 0.0f).RotateAngleAxis(-45.0f, FVector(0.0f, 0.0f, 1.0f));
	AddActorWorldTransform(FTransform(vec));
}

void APlayerCamera::MoveRightward(float Val)
{
	FVector vec = FVector( 0.0f, speed * Val, 0.0f).RotateAngleAxis(-45.0f, FVector(0.0f, 0.0f, 1.0f));
	AddActorWorldTransform(FTransform(vec));
}

