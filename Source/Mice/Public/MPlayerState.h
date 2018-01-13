// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MPlayerState.generated.h"

UENUM(BlueprintType)
enum class ETeam : uint8
{
	T_Blue 	UMETA(DisplayName = "Blue"),
	T_Red 	UMETA(DisplayName = "Red")
};


DECLARE_DELEGATE(FOnPlayerColorChange)
/**
 * 
 */
UCLASS()
class MICE_API AMPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMPlayerState();
	UPROPERTY(Transient, ReplicatedUsing = OnRep_TeamColor, VisibleAnywhere, BlueprintReadWrite, Category = "Team")
		ETeam team;

	FOnPlayerColorChange OnColorChangeDelegate;

	//Do not store the team arg in the player pawn
	void UpdatePlayerTeam();

	UFUNCTION()
		void OnRep_TeamColor();

	void SetTeam(ETeam tm);

protected:
	virtual void BeginPlay() override;
};
