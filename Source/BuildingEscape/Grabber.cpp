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

	// (0,0,0) 위치에서 PlayerViewPointLocation 까지의 벡터 + PlayerViewPointRotation 벡터에 Reach 값을 곱함.
	// 이는 Player의 View Point에서 Reach 만큼의 거리 앞 까지 Line을 의미함.
	// 이때, Rotation의 경우 unit 벡터 이므로 Reach 값을 곱해(스칼라 곱) 길이를 늘려준 것임.
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
	// 특정 거리에 대해 Ray-cast
	FCollisionQueryParams TraceParams( FName( TEXT( "" ) ), false, GetOwner() );

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), // 오브젝트의 콜리전 타입. ECollisionChannel은 enum class
		TraceParams
	);

	// Hit 오브젝트 체크
	AActor* HitActor = Hit.GetActor();

	if( HitActor )
	{
		UE_LOG( LogTemp, Warning, TEXT( "Hit : %s" ), *(HitActor->GetName()) );
	}
}

