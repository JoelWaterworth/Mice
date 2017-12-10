// Copyright 2016 Allegorithmic Inc. All rights reserved.
// File: SubstanceCoreModule.h

#pragma once
#include "ISubstanceCore.h"
#include "Ticker.h"
#include "SubstanceSettings.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#endif

#define LOCTEXT_NAMESPACE "SubstanceModule"

class FSubstanceCoreModule : public ISubstanceCore
{
	//Substance Core Initialization
	virtual void StartupModule() override;

	//Module cleanup
	virtual void ShutdownModule() override;

	//Returns the max output size based on the set Substance Engine
	virtual uint32 GetMaxOutputTextureSize() const override;

	//Module update
	virtual bool Tick(float DeltaTime);

	//Registers the Substance settings to appear within the project settings (Editor Only)
	void RegisterSettings()
	{
#if WITH_EDITOR
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->RegisterSettings("Project", "Plugins", "Substance",
			                                 LOCTEXT("SubstanceSettingsName", "Substance"),
			                                 LOCTEXT("SubstanceSettingsDescription", "Configure the Substance plugin"),
			                                 GetMutableDefault<USubstanceSettings>()
			                                );
		}
#endif
	}

	//Unregister the Substance settings from the project settings (Editor Only)
	void UnregisterSettings()
	{
#if WITH_EDITOR
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->UnregisterSettings("Project", "Plugins", "Substance");
		}
#endif
	}

#if WITH_EDITOR
	void OnBeginPIE(const bool bIsSimulating);
	void OnEndPIE(const bool bIsSimulating);

public:
	bool isPie()
	{
		return PIE;
	}
#endif //WITH_EDITOR

private:
	//Tick delegate to register module update
	FTickerDelegate TickDelegate;

	//Callback for when the game world is loaded and initialized
	static void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS);

	//Callback for when a never level is added
	static void OnLevelAdded(ULevel* Level, UWorld* World);

	//Stores if game is currently in Play In Editor mode
	bool PIE;

	//Used to write out the Substance settings for debug purposes
	void LogSubstanceSettings();
};

#undef LOCTEXT_NAMESPACE
