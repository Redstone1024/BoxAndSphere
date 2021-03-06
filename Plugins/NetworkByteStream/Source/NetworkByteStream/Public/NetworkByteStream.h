// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

NETWORKBYTESTREAM_API DECLARE_LOG_CATEGORY_EXTERN(LogNetworkByteStream, Log, All);

class FNetworkByteStreamModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
