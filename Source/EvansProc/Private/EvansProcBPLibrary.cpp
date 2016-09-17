// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "EvansProcPrivatePCH.h"
#include "EvansProcBPLibrary.h"


UEvansProcBPLibrary::UEvansProcBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

float UEvansProcBPLibrary::EvansProcSampleFunction(float Param)
{
	return -1;
}

TArray<FVertexData> UEvansProcBPLibrary::test123(float vertexSpacing, float xWidth, float yWidth)
{
	TArray<FVertexData> test;
	FVertexData test12;
	test12.XPosVertexData.Add(FVector(0, 0, 0));
	test.Add(test12);

	return  test;
}