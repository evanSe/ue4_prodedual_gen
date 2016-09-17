// Fill out your copyright notice in the Description page of Project Settings.
#include "EvansProcPrivatePCH.h"
#include "GenLandscapeActor.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "OpenSimplexNoiseComponent.h"

// Sets default values
AGenLandscapeActor::AGenLandscapeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProcedualMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcedualMeshComponent"));
	RootComponent = ProcedualMesh;

	// Register OpenSimplexNoiseComponentComponent to Actor
	OpenNoise = CreateDefaultSubobject<UOpenSimplexNoiseComponent>(TEXT("OpenSimplexNoiseComponent"));
	OpenNoise->RegisterComponent();
}

// Called when the game starts or when spawned
void AGenLandscapeActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGenLandscapeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<FVertexPosData> AGenLandscapeActor::GenerateVertexData(float vertexSpacing, float xWidth, float yWidth)
{
	
	TArray<FVertexPosData> VertexArray;
	for (int yIndex = 0; yIndex < yWidth; yIndex++)
	{
		FVertexPosData VertexRow;

		for (int xIndex = 0; xIndex < xWidth; xIndex++)
		{
			FVector VertexPosition = FVector(xIndex * vertexSpacing, yIndex * vertexSpacing, vertexSpacing);
			VertexRow.XPosVertexData.Add(VertexPosition);
		}
		VertexArray.Add(VertexRow);
	}
	return  VertexArray;
}

void AGenLandscapeActor::CreateMesh(TArray<FVertexPosData> TriangleVertexData)
{
	int ySize = TriangleVertexData.Num();
	int indexCount = 0;
	TArray<FVector> TriangleData;
	for (int yIndex = 0; yIndex < ySize; yIndex++)
	{
		
		int xSize = TriangleVertexData[yIndex].XPosVertexData.Num();
		if (yIndex != (ySize - 1))
		{
			for (int xIndex = 0; xIndex < xSize; xIndex++)
			{
				if (xIndex != (xSize - 1))
				{
					// Top Triangles
					TriangleData.Add(TriangleVertexData[yIndex].XPosVertexData[xIndex]);
					TriangleData.Add(TriangleVertexData[yIndex + 1].XPosVertexData[xIndex +1]);
					TriangleData.Add(TriangleVertexData[yIndex].XPosVertexData[xIndex + 1]);

					// Bottom Triangles
					TriangleData.Add(TriangleVertexData[yIndex + 1].XPosVertexData[xIndex + 1]);
					TriangleData.Add(TriangleVertexData[yIndex].XPosVertexData[xIndex]);
					TriangleData.Add(TriangleVertexData[yIndex + 1].XPosVertexData[xIndex]);
					
				}
			}
		}
	}
	CreateMeshSection(TriangleData, indexCount++);
}

void AGenLandscapeActor::CreateMeshSection(TArray<FVector> TriangleData, int32 index)
{
	TArray<FVector> vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UV0;
	TArray<FColor> vertexColors;
	TArray<FVector> normals;
	TArray<FProcMeshTangent> tangents;

	int increase = 1;
	bool yes = false;
	for (int triIndex = 0; triIndex < TriangleData.Num(); triIndex++)
	{
		vertices.Add(TriangleData[triIndex]);
		Triangles.Add(triIndex);

		// Todo Calculate uv Data properly
		UV0.Add(FVector2D(TriangleData[triIndex].X, TriangleData[triIndex].Y));
		// Todo Calculate Normal fully.
		normals.Add(TriangleData[triIndex]);
		
	}

	// Create Mesh section
	ProcedualMesh->CreateMeshSection(index, vertices, Triangles, normals, UV0, vertexColors, tangents, true);
	
	// Apply Material to mesh section
	ProcedualMesh->SetMaterial(0, MeshMaterial);

	// Make sure it casts shadows
	ProcedualMesh->CastShadow = true;
	ProcedualMesh->bCastDynamicShadow = true;
}

TArray<FVertexPosData> AGenLandscapeActor::CreateOpenSimplexData(TArray<FVertexPosData> PosData, float divide,float seed, float period, float octaves, float persistance, float lacunarity)
{
	TArray<FVertexPosData> NewPosData = PosData;
	for (int yIndex = 0; yIndex < NewPosData.Num(); yIndex++)
	{
		for (int xIndex = 0; xIndex <NewPosData[yIndex].XPosVertexData.Num(); xIndex++)
		{
			float noise = OpenNoise->Get4dNoise(seed, period, octaves, persistance, lacunarity, NewPosData[yIndex].XPosVertexData[xIndex].X, NewPosData[yIndex].XPosVertexData[xIndex].Y, NewPosData[yIndex].XPosVertexData[xIndex].Z, 0) / divide;
			
			NewPosData[yIndex].XPosVertexData[xIndex].Z = NewPosData[yIndex].XPosVertexData[xIndex].Z * noise;
		}
	}
	return NewPosData;
}

TArray<FVertexPosData> AGenLandscapeActor::Applyfbm(TArray<FVertexPosData> PosData, float divide, float seed, float period, float octaves, float persistance, float lacunarity)
{
	TArray<FVertexPosData> NewPosData = PosData;

	for (int yIndex = 0; yIndex < NewPosData.Num(); yIndex++)
	{
		for (int xIndex = 0; xIndex < NewPosData[yIndex].XPosVertexData.Num(); xIndex++)
		{
			float sum = 0;
			float freq = 1;
			float ampl = 1;
			float gain = 0.5;
			for (int i = 0; i < octaves; i++)
			{
				float noise = OpenNoise->Get4dNoise(seed, period * freq, octaves, persistance, lacunarity, NewPosData[yIndex].XPosVertexData[xIndex].X, NewPosData[yIndex].XPosVertexData[xIndex].Y, NewPosData[yIndex].XPosVertexData[xIndex].Z, 0) / divide;
				sum += noise*ampl;
				freq *= lacunarity;
				ampl *= gain;
			}
			

			NewPosData[yIndex].XPosVertexData[xIndex].Z = NewPosData[yIndex].XPosVertexData[xIndex].Z * sum;
		}
	}
	return NewPosData;
}
/*
TArray<FVertexPosData> AGenLandscapeActor::billowedNoise()
{

}

TArray<FVertexPosData> AGenLandscapeActor::ridgedNoise()
{

}

TArray<FVertexPosData> AGenLandscapeActor::iqNoise()
{

}
*/