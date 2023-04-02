#include "Weapons/CPistol.h"

#include "Characters/CPlayer.h"
#include "Interfaces/IWeapon.h"
#include "Weapons/CBullet.h"

#include "GameFramework/Character.h"
#include "Components/DecalComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceConstant.h"

ACPistol::ACPistol()
{
	PrimaryActorTick.bCanEverTick = true;

	// [Scene]
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	// [Mesh]
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(Scene);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> meshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/Pistol/Mesh/SK_Pistol.SK_Pistol'"));
	Mesh->SetSkeletalMesh(meshAsset.Object);

	Mesh->SetRelativeRotation(FRotator(0, -90, 0));

	// [Bullet]
	ConstructorHelpers::FClassFinder<ACBullet> bulletClassAsset(TEXT("/Script/Engine.Blueprint'/Game/_Project/Weapons/BP_CBullet.BP_CBullet_C'"));
	if (bulletClassAsset.Succeeded())
		BulletClass = bulletClassAsset.Class;

	// [CameraShake]
	ConstructorHelpers::FClassFinder<UCameraShakeBase> cameraShakeAsset(TEXT("/Script/Engine.Blueprint'/Game/_Project/Weapons/BP_CameraShake.BP_CameraShake_C'"));
	if (cameraShakeAsset.Succeeded())
		CameraShakeClass = cameraShakeAsset.Class;

	// [Particle]
	ConstructorHelpers::FObjectFinder<UParticleSystem> ImpactParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Particles_Rifle/Particles/VFX_Impact_Default.VFX_Impact_Default'"));
	if (ImpactParticleAsset.Succeeded())
		ImpactParticle = ImpactParticleAsset.Object;

	// [Decal]
	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> decalAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_Decal_Inst.M_Decal_Inst'"));
	if (decalAsset.Succeeded())
		DecalMaterial = decalAsset.Object;
}

void ACPistol::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void ACPistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ACPistol* ACPistol::Spawn(UWorld* InWorld, ACharacter* InOwnerCharacter)
{
	FActorSpawnParameters spawnParam;
	spawnParam.Owner = InOwnerCharacter;
	return InWorld->SpawnActor<ACPistol>(spawnParam);
}

void ACPistol::Begin_Fire()
{
	bFiring = true;

	CurrentPitch = 0.f;

	Firing();
}

void ACPistol::End_Fire()
{
	bFiring = false;
}

void ACPistol::Firing()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, "ACPistol::Firing");

	// [Get Aim Info]
	IIWeapon* weaponInterface = Cast<IIWeapon>(OwnerCharacter);
	if (weaponInterface == nullptr) return;
	
	FVector start, end, direction;
	weaponInterface->GetAimInfo(start, end, direction);

	// [Camera Shake]
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (player != nullptr)
	{
		APlayerController* controller = player->GetController<APlayerController>();
	
		if (CameraShakeClass != nullptr)
			controller->ClientPlayCameraShake(CameraShakeClass);
	}

	// [Spawn Bullet]
	FVector muzzleLocation = Mesh->GetSocketLocation("Muzzle");
	if (BulletClass != nullptr)
		GetWorld()->SpawnActor<ACBullet>(BulletClass, muzzleLocation, direction.Rotation());

	// [Raising Pitch]
	CurrentPitch -= PitchSpeed * GetWorld()->GetDeltaSeconds();
	OwnerCharacter->AddControllerPitchInput(CurrentPitch);

	// [LineTrace]
	FHitResult hitResult;
	FCollisionQueryParams collisionQueryParams;
	collisionQueryParams.AddIgnoredActor(this);
	collisionQueryParams.AddIgnoredActor(OwnerCharacter);
	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, collisionQueryParams))
	{
		AStaticMeshActor* staticMeshActor = Cast<AStaticMeshActor>(hitResult.GetActor());
		if (staticMeshActor != nullptr)
		{
			UStaticMeshComponent* staticMeshComp = Cast<UStaticMeshComponent>(staticMeshActor->GetRootComponent());
			if (staticMeshComp != nullptr)
			{
				// [Spawn Decal & Impact Particle]
				FRotator decalRotator = hitResult.ImpactNormal.Rotation();
				UDecalComponent* decalComp = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), Cast<UMaterialInterface>(DecalMaterial), FVector(5), hitResult.Location, decalRotator, 10.f);
				decalComp->SetFadeScreenSize(0);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, hitResult.Location, decalRotator, true);

				// [Take Force]
				if (staticMeshComp->BodyInstance.bSimulatePhysics == true)
				{
					direction = staticMeshActor->GetActorLocation() - OwnerCharacter->GetActorLocation();
					direction.Normalize();
					staticMeshComp->AddImpulseAtLocation(direction * 30000.f, OwnerCharacter->GetActorLocation());
					return;
				} // if (staticMeshComp->BodyInstance.bSimulatePhysics == true)
			} // if (staticMeshComp != nullptr)
		} // if (staticMeshActor != nullptr)
	} // if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_PhysicsBody, collisionQueryParams))
}
