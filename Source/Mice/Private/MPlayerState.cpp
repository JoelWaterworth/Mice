// Fill out your copyright notice in the Description page of Project Settings.

#include "MPlayerState.h"



AMPlayerState::AMPlayerState()
{

}

void AMPlayerState::UpdatePlayerTeam()
{
	OnColorChangeDelegate.ExecuteIfBound();
}

void AMPlayerState::BeginPlay()
{
	Super::BeginPlay();
	UpdatePlayerTeam();
}
