#include "Weapons/CBullet.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"

ACBullet::ACBullet()
{
	// [Mesh]
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(TEXT("/Script/Engine.StaticMesh'/Game/_Project/Meshes/Sphere.Sphere'"));
	if (meshAsset.Succeeded())
		Mesh->SetStaticMesh(meshAsset.Object);

	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> materialAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/_Project/Materials/M_CBullet_Inst.M_CBullet_Inst'"));
	if (materialAsset.Succeeded())
		Mesh->SetMaterial(0, materialAsset.Object);

	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetRelativeScale3D(FVector(1.f, 0.025f, 0.025f));

	Mesh->CastShadow = 0;

	// [Projectile]
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");

	Projectile->InitialSpeed = 2e+4f;
	Projectile->MaxSpeed = 2e+4f;
	Projectile->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void ACBullet::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(3.f);
}
