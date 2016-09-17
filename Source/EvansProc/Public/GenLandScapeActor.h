// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GenLandscapeActor.generated.h"


USTRUCT(BlueprintType)
struct FVertexPosData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandScapeGeneeration")
		TArray<FVector> XPosVertexData;
};

UCLASS()
class AGenLandscapeActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGenLandscapeActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "LandScapeGeneeration")
		TArray<FVertexPosData> GenerateVertexData(float vertexSpacing, float xWidth, float yWidth);

	UFUNCTION(BlueprintCallable, Category = "LandScapeGeneeration")
		void CreateMesh(TArray<FVertexPosData> TriangleVertexData);

	UFUNCTION(BlueprintCallable, Category = "LandScapeGeneeration")
		TArray<FVertexPosData> CreateOpenSimplexData(TArray<FVertexPosData> PosData, float divide, float seed, float period, float octaves, float persistance, float lacunarity);
	UFUNCTION(BlueprintCallable, Category = "LandScapeGeneeration")
		TArray<FVertexPosData> Applyfbm(TArray<FVertexPosData> PosData, float divide, float seed, float period, float octaves, float persistance, float lacunarity);

	void CreateMeshSection(TArray<FVector> TriangleData, int32 index);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandScapeGeneeration")
		class UProceduralMeshComponent* ProcedualMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandScapeGeneeration")
		class UOpenSimplexNoiseComponent* OpenNoise;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandScapeGeneeration")
		class UMaterial* MeshMaterial;
};
