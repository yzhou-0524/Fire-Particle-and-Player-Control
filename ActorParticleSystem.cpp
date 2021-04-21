// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorParticleSystem.h"
#include "Components/BillboardComponent.h"
#include <string>
#include <algorithm> // max
#include "ez/Util.h"
#include "ez/Texture.h"
#include "ez/Random.h"
#include "Kismet/GameplayStatics.h"

void FBillboardParticle::update(float dt) {
	vel += acc * dt * 5;
	pos += vel * dt;
	age += dt;
	if (billboard) billboard->SetRelativeLocation(pos);
}

FBillboardParticle& UBillboardParticles::nextParticle() {
	++currIndex %= particles.Num();
	auto& p = particles[currIndex];
	p.age = 0.;
	return p;
}

void UBillboardParticles::update(float dt) {
	for (auto& p : particles) p.update(dt);
}

// Sets default values
AActorParticleSystem::AActorParticleSystem()
{
	PrimaryActorTick.bCanEverTick = true;

	// This is static so it only gets created once over the program lifetime
		/*static ConstructorHelpers::FObjectFinder<UTexture2D> asset(TEXT("Games/Fireworks/Texture/fireworks"));
		if (asset.Succeeded()) {
			Billboard->Sprite = asset.Object;
		}*/

	// Generate a texture to use as billboard sprite
	ez::Texture2D tex(32, 32);
	for (unsigned j = 0; j < tex.height(); ++j) {
		for (unsigned i = 0; i < tex.width(); ++i) {

			float u = float(i) / tex.width();
			float v = float(j) / tex.height();
			float x = 2. * u - 1.;
			float y = 2. * v - 1.;

			// Radius squared from center of sprite
			float rsqr = x * x + y * y;

			// Set pixel color and alpha
			auto* pixel = tex.pixel(i, j);
			pixel[0] = 0 * u;//blue
			pixel[1] = 0 * v;//green
			pixel[2] = 255;//red
			pixel[3] = rsqr < 1. ? 255 : 0; // cut-out a circle
		}
	}

	//auto* utex = tex.newUTexture2D();

	EZ_CREATE_DEFAULT_SUBOBJECT(particleSystem);
	RootComponent = particleSystem;

	auto& particles = particleSystem->particles;

	particles.SetNum(50);//this line can be the final firework

	// Initialize all the particles
	for (int i = 0; i < particles.Num(); ++i) 
	//for (int i = 101; i >0; --i)
	{
		auto& p = particles[i];
		
		auto name = FName(("p" + std::to_string(i)).c_str());// Generate a name for the particle (req'd by CreateDefaultSubobject)


		p.billboard = CreateDefaultSubobject<std::decay<decltype(*p.billboard)>::type>(name);// Create the billboard

		// Set billboard sprite to our texture generated above
		//p.billboard->Sprite = utex;
		static ConstructorHelpers::FObjectFinder<UTexture2D> asset(TEXT("/Game/Fireworks_BP/fire3"));
		if (asset.Succeeded()) {
			//auto* pixel = asset.Object;
			//pixel[0] = asset.Object->GetWidth() / 5.0f; // try subUV
			p.billboard->Sprite = asset.Object;
		}

		p.billboard->bHiddenInGame = false;
	
		p.billboard->SetupAttachment(particleSystem);// attach particle to particle system component

		p.billboard->SetMaterial(0, Material);// attach material to the particle system component
	}

}

// Called when the game starts or when spawned
void AActorParticleSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorParticleSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (rng.P(1)) {
		auto& p = particleSystem->nextParticle();
		p.pos = rng.ball<FVector>() * 0.01f;	// random position within a sphere
		p.vel = rng.ball<FVector>() * 30.; // random isotropic velocity vector
		p.acc = FVector(0, 0, 0); // no gravity
	}

	// Add some jitter to each particle (based on a random walk)
	for (auto& p : particleSystem->particles) {
		p.vel += rng.ball<FVector>() * 30.;
	}

	// Update particle positions
	particleSystem->update(DeltaTime);

	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Emitter_Projectile, this->GetTransform());


}

