// Copyrights Jamin Heo 2020 

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player's view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( 
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation 
	);

	/*
	UE_LOG( LogTemp, Warning, TEXT( "Location : %s / Rotation : %s" ), 
		*PlayerViewPointLocation.ToString(), 
		*PlayerViewPointRotation.ToString() 
	);
	*/

	// (0,0,0) ��ġ���� PlayerViewPointLocation ������ ���� + PlayerViewPointRotation ���Ϳ� Reach ���� ����.
	// �̴� Player�� View Point���� Reach ��ŭ�� �Ÿ� �� ���� Line�� �ǹ���.
	// �̶�, Rotation�� ��� unit ���� �̹Ƿ� Reach ���� ����(��Į�� ��) ���̸� �÷��� ����.
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f
	);

	FHitResult Hit;
	// Ư�� �Ÿ��� ���� Ray-cast
	FCollisionQueryParams TraceParams( FName( TEXT( "" ) ), false, GetOwner() );

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), // ������Ʈ�� �ݸ��� Ÿ��. ECollisionChannel�� enum class
		TraceParams
	);

	// Hit ������Ʈ üũ
	AActor* HitActor = Hit.GetActor();

	if( HitActor )
	{
		UE_LOG( LogTemp, Warning, TEXT( "Hit : %s" ), *(HitActor->GetName()) );
	}
}

