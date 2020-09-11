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

}

// Physics handle 컴포넌트 체크
void UGrabber::FindPhysicsHandle()
{

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if( PhysicsHandle )
	{
	}
	else
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
	UE_LOG( LogTemp, Warning, TEXT( "Grabber pressed!" ) );

	GetFirstPhysicsBodyInReach();
}

void UGrabber::Release()
{
	UE_LOG( LogTemp, Warning, TEXT( "Grabber released!" ) );
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
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
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	FHitResult Hit;
	// 특정 거리에 대해 Ray-cast
	FCollisionQueryParams TraceParams( FName( TEXT( "" ) ), false, GetOwner() );

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ), // 오브젝트의 콜리전 타입. ECollisionChannel은 enum class
		TraceParams
	);

	// Hit 오브젝트 체크
	AActor* HitActor = Hit.GetActor();

	if( HitActor )
	{
		UE_LOG( LogTemp, Warning, TEXT( "Hit : %s" ), *(HitActor->GetName()) );
	}

	return Hit;
}

