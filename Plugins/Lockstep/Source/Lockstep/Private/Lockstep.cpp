// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Lockstep.h"
#include "LockstepSubsystem.h"
#include "Developer/Settings/Public/ISettingsModule.h"

DEFINE_LOG_CATEGORY(LogLockstep);

#define LOCTEXT_NAMESPACE "FLockstepModule"

void FLockstepModule::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "Plugins", "Lockstep",
			LOCTEXT("RuntimeSettingsName", "Lockstep"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Lockstep plugin"),
			GetMutableDefault<ULockstepSubsystem>()
		);
	}
}

void FLockstepModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Lockstep");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLockstepModule, Lockstep)