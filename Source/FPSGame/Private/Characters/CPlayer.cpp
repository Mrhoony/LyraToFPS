#include "Characters/CPlayer.h"

#include "Characters/CAniminstance.h"
#include "Weapons/CPistol.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// [Mesh]
	ConstructorHelpers::FObjectFinder<USkeletalMesh> meshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Heroes/Mannequin/Meshes/SKM_Manny.SKM_Manny'")); //TODO
	if (meshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(meshAsset.Object);

	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));

	ConstructorHelpers::FClassFinder<UCAnimInstance> animInstanceClass(TEXT("/Script/Engine.AnimBlueprint'/Game/_Project/Characters/AB_CPlayer.AB_CPlayer_C'"));
	if(animInstanceClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(animInstanceClass.Class);

	// [SpringArm]
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->SetRelativeLocation(FVector(0, 0, 100));
	SpringArm->TargetArmLength = 200.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	
	// [CharacterMovement]
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	// [Camera]
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	// [Pistol]
	Pistol = ACPistol::Spawn(GetWorld(), this);
	Pistol->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "weapon_r");
}

void ACPlayer::GetAimInfo(FVector& OutAimStart, FVector& OutAimEnd, FVector& OutAimDirection)
{
	OutAimDirection = Camera->GetForwardVector();

	FTransform transform = Camera->GetComponentToWorld();
	OutAimStart = transform.GetLocation() + OutAimDirection * 100.f;

	FVector recoilCone = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(OutAimDirection, 0.2f);

	OutAimEnd = OutAimStart + recoilCone * 3000;
}

void ACPlayer::OnTarget()
{
}

void ACPlayer::OffTarget()
{
}



void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ACPlayer::OnFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &ACPlayer::OffFire);
}

void ACPlayer::OnMoveForward(float InAxis)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	AddControllerYawInput(InAxis);
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	AddControllerPitchInput(InAxis);
}

void ACPlayer::OnFire()
{
	//CurWeapon->Begin_Fire();
	Pistol->Begin_Fire();
}

void ACPlayer::OffFire()
{
	//CurWeapon->End_Fire();
}
