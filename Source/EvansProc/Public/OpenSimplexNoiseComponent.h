#pragma once

#include "Components/ActorComponent.h"
#include "OpenSimplexNoiseComponent.generated.h"

#define SIMPLEX_STRETCH_3D (-1.0f / 6.0f)
#define SIMPLEX_SQUASH_3D  (+1.0f / 3.0f)

// Normalization constant tested using over 4 billion evaluations to bound
// range within [-1,1]. This is a safe upper-bound. Actual min/max values
// found over the course of the 4 billion evaluations were
// -28.12974224468639 (min) and 28.134269887817773 (max).
#define SIMPLEX_NORMALIZATION 28.25f // normalize into the range [-1,+1]

extern "C" {
	struct osn_context;
	int open_simplex_noise(int64_t seed, struct osn_context **ctx);
	void open_simplex_noise_free(struct osn_context *ctx);
	int open_simplex_noise_init_perm(struct osn_context *ctx, int16_t p[], int nelements);
	double open_simplex_noise2(struct osn_context *ctx, double x, double y);
	double open_simplex_noise3(struct osn_context *ctx, double x, double y, double z);
	double open_simplex_noise4(struct osn_context *ctx, double x, double y, double z, double w);
}

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UOpenSimplexNoiseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenSimplexNoiseComponent();


	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	// Disabled Tick, Not needed
	//virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


	void set_Seed(int seed);
	int get_seed();

	UFUNCTION(BlueprintCallable, Category = "Noise")
	float Get4dNoise(float seed, float period, float octaves, float persistance, float lacunarity, float x, float y, float z, float w);

	float GenerateNoise4d(osn_context * _context, float x, float y, float z, float w, float period, float octaves, float persistance, float lacunarity);

};