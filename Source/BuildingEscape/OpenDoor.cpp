// Copyrights Jamin Heo 2020 


#include "OpenDoor.h"
#include "GameFrameWork/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
// GetOwner를 BeginPlay가 아닌 생성자에 선언할 경우 GetOwner 호출 시 NULL이 리턴될 경우 오류가 발생할 가능성이 있으므로 주의.
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	if( !PressurePlate )
	{
		UE_LOG( LogTemp, Error, TEXT( "%s has OpenDoor Component, but has no Pressure plate." ), *GetOwner()->GetName() );
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// if(ptr) : 포인터가 NULL 포인터인지 여부를 간단하게 체크할 수 있음. null이 아닌 경우 조건을 만족.
	if( PressurePlate && PressurePlate->IsOverlappingActor( ActorThatOpens ) )
	{
		OpenDoor( DeltaTime );
	}
}

void UOpenDoor::OpenDoor( float DeltaTime )
{
	CurrentYaw = FMath::FInterpTo( CurrentYaw, TargetYaw, DeltaTime, 2 );
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation( DoorRotation );

	//UE_LOG( LogTemp, Warning, TEXT( "Yaw is : %f" ), GetOwner()->GetActorRotation().Yaw );
}

// Frame에 독립적으로 수행. 상수 스텝 만큼 interpolate 0초 : 0도 -> 1초 : 45도 -> 2초 : 90도 (Linear Interpolation)
// OpenDoor.Yaw = FMath::FInterpConstantTo( CurrentYaw, ActorRotation.Yaw + TargetYaw, DeltaTime, 45 );
// 더 부드럽게 Interpolation 하고 싶은 경우
// CurrentYaw = FMath::FInterpTo( CurrentYaw, TargetYaw, DeltaTime, 2 );

