// Fill out your copyright notice in the Description page of Project Settings.

#include "MPlayerState.h"
#include "MPlayerController.h"
#include "UnrealNetwork.h"


AMPlayerState::AMPlayerState()
{

}

void AMPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPlayerState, team);
}

void AMPlayerState::OnRep_TeamColor()
{
	UpdatePlayerTeam();
}

void AMPlayerState::UpdatePlayerTeam()
{
	AMPlayerController* playercontroller = Cast<AMPlayerController>(GetOwner());
	if (playercontroller != NULL) {
		playercontroller->UpdatePlayerTeam(team);
	}
}

void AMPlayerState::SetTeam(ETeam tm) {
	team = tm;
	UpdatePlayerTeam();
}

void AMPlayerState::BeginPlay()
{
	Super::BeginPlay();
	UpdatePlayerTeam();
}
