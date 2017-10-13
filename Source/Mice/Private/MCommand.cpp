// Fill out your copyright notice in the Description page of Project Settings.

#include "MCommand.h"

void UMCommand::Run()
{
	unit->MoveTo(path);
}
