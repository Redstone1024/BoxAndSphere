// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NetworkByteStream.h"

DEFINE_LOG_CATEGORY(LogNetworkByteStream);

#define LOCTEXT_NAMESPACE "FNetworkByteStreamModule"

void FNetworkByteStreamModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FNetworkByteStreamModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNetworkByteStreamModule, NetworkByteStream)