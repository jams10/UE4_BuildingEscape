// Copyrights Jamin Heo 2020 

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if( !PhysicsHandle ) return;
	// ���� Grab�� ������
	if( PhysicsHandle->GrabbedComponent )
	{
		// Grab�� ������ ��ġ�� �÷��̾� Reach�� ��(Linetrace�� ��)���� ������.
		PhysicsHandle->SetTargetLocation( GetPlayerReach() );
		// Grab�� ������ ȸ���� ���� Player Rotation���� �ٲپ� ��.
		PhysicsHandle->SetTargetRotation( GetPlayerWorldRotation() );
	}
}

// Physics handle ������Ʈ üũ
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if( PhysicsHandle == nullptr)
	{
		UE_LOG( LogTemp, Error, TEXT( "%s has no Physics Component." ), *(GetOwner()->GetName()) );
	}
}
// InputComponent �¾�
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if( InputComponent )
	{
		// ������ �Ű��������� Grab �Լ��� 'ȣ��' �ϴ� ���� �ƴ�, ��� �Լ��� ����� ���� ����Ű�Ƿ�, �Լ� �ڿ� ()�� ���� �ʴ´�.
		// �� Action Key�� �̺�Ʈ �Լ��� �����ִ� ���̴�.
		InputComponent->BindAction( "Grab", IE_Pressed, this, &UGrabber::Grab );
		InputComponent->BindAction( "Grab", IE_Released, this, &UGrabber::Release );
	}
}

void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// HitResult�� ���� Actor�� ������. ������ Ÿ��.
	// Hit�� Actor�� ������, Physics handle�� ����.
	if( ActorHit )
	{
		if( !PhysicsHandle ) return;
		// GrabComponentAtLocationWithRotation = Rotation�� Contraint(����)�� �ɾ� Component�� Grab
		// �׳� Location�� �ִ� �Լ� ����� ���, ��ü�� �������� �� �¿�� ����ؼ� ������.
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			GetPlayerReach(),
			GetPlayerWorldRotation()
		);
	}
}

void UGrabber::Release()
{
	if( !PhysicsHandle ) return;
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	// Ư�� �Ÿ��� ���� Ray-cast
	FCollisionQueryParams TraceParams( FName( TEXT( "" ) ), false, GetOwner() );

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerWorldPosition(),
		GetPlayerReach(),
		FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ), // ������Ʈ�� �ݸ��� Ÿ��. ECollisionChannel�� enum class
		TraceParams
	);

	return Hit;
}

FVector UGrabber::GetPlayerReach() const
{
	// Get player's view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// (0,0,0) ��ġ���� PlayerViewPointLocation ������ ���� + PlayerViewPointRotation ���Ϳ� Reach ���� ����.
	// �̴� Player�� View Point���� Reach ��ŭ�� �Ÿ� �� ���� Line�� �ǹ���.
	// �̶�, Rotation�� ��� unit ���� �̹Ƿ� Reach ���� ����(��Į�� ��) ���̸� �÷��� ����.
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabber::GetPlayerWorldPosition() const
{
	// Get player's view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FRotator UGrabber::GetPlayerWorldRotation() const
{
	// Get player's view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointRotation;
}

