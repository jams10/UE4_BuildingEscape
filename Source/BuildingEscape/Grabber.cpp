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
	// 무언가 Grab을 했으면
	if( PhysicsHandle->GrabbedComponent )
	{
		// Grab한 엑터의 위치를 플레이어 Reach의 끝(Linetrace의 끝)으로 변경함.
		PhysicsHandle->SetTargetLocation( GetPlayerReach() );
		// Grab한 엑터의 회전값 또한 Player Rotation으로 바꾸어 줌.
		PhysicsHandle->SetTargetRotation( GetPlayerWorldRotation() );
	}
}

// Physics handle 컴포넌트 체크
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if( PhysicsHandle == nullptr)
	{
		UE_LOG( LogTemp, Error, TEXT( "%s has no Physics Component." ), *(GetOwner()->GetName()) );
	}
}
// InputComponent 셋업
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if( InputComponent )
	{
		// 마지막 매개변수에서 Grab 함수를 '호출' 하는 것이 아닌, 어느 함수로 가라는 것을 가리키므로, 함수 뒤에 ()를 적지 않는다.
		// 즉 Action Key와 이벤트 함수를 묶어주는 것이다.
		InputComponent->BindAction( "Grab", IE_Pressed, this, &UGrabber::Grab );
		InputComponent->BindAction( "Grab", IE_Released, this, &UGrabber::Release );
	}
}

void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// HitResult에 대한 Actor를 리턴함. 포인터 타입.
	// Hit한 Actor가 있으면, Physics handle을 붙임.
	if( ActorHit )
	{
		if( !PhysicsHandle ) return;
		// GrabComponentAtLocationWithRotation = Rotation에 Contraint(제약)을 걸어 Component를 Grab
		// 그냥 Location만 있는 함수 사용할 경우, 물체를 집어들었을 때 좌우로 계속해서 스윙함.
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
	// 특정 거리에 대해 Ray-cast
	FCollisionQueryParams TraceParams( FName( TEXT( "" ) ), false, GetOwner() );

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerWorldPosition(),
		GetPlayerReach(),
		FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ), // 오브젝트의 콜리전 타입. ECollisionChannel은 enum class
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

	// (0,0,0) 위치에서 PlayerViewPointLocation 까지의 벡터 + PlayerViewPointRotation 벡터에 Reach 값을 곱함.
	// 이는 Player의 View Point에서 Reach 만큼의 거리 앞 까지 Line을 의미함.
	// 이때, Rotation의 경우 unit 벡터 이므로 Reach 값을 곱해(스칼라 곱) 길이를 늘려준 것임.
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

