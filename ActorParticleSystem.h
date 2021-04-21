// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"// contains some basic type like Type
#include "GameFramework/Actor.h"
#include "ez/Random.h"
#include "MaterialExpressionIO.h"
#include "Materials/Material.h"
#include "Engine/Texture2D.h"
#include "ActorParticleSystem.generated.h"


USTRUCT()
struct FBillboardParticle {
	GENERATED_BODY()

	FVector pos, vel, acc;
	float age = 0.;

	// Update position of particle
	void update(float dt);

	UPROPERTY()
		class UBillboardComponent* billboard;
};


UCLASS()
class UBillboardParticles : public USceneComponent {
public:
	GENERATED_BODY()

		// Array of all the particles
		UPROPERTY()
		TArray<FBillboardParticle> particles;

	// Return oldest particle from set
	FBillboardParticle& nextParticle();

	// Update all particles
	void update(float dt);

private:
	int currIndex = -1; // read/write tap into array
};

UCLASS()
class FIREWORKS_03_API AActorParticleSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorParticleSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UBillboardParticles* particleSystem;
	// Random number generator used for particle motion
	ez::Random<> rng;

protected:
	/*UPROPERTY(EditDefaultsOnly, Category = "Material")
		UParticleSystem* Emitter_Projectile;*/

	UPROPERTY(EditAnywhere)
		//FColorMaterialInput* FExpressionInput;
		UMaterial* Material;

	UPROPERTY(EditAnywhere)
		UTexture2D* Texture;
};
