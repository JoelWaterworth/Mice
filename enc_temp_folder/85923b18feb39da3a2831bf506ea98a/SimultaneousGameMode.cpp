// Fill out your copyright notice in the Description page of Project Settings.

#include "SimultaneousGameMode.h"
#include "PlayerCamera.h"


ASimultaneousGameMode::ASimultaneousGameMode()
{
	DefaultPawnClass = APlayerCamera::StaticClass();
}