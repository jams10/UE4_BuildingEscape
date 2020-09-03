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

	ActorRotation = GetOwner()->GetActorRotation();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// UE_LOG(LogTemp, Warning, TEXT("Yaw is : %f"), GetOwner()->GetActorLocation().Yaw);

	float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator OpenDoor( 0.f, 90.f, 0.f );
	// Frame에 독립적으로 수행. 상수 스텝 만큼 interpolate 0초 : 0도 -> 1초 : 45도 -> 2초 : 90도 (Linear Interpolation)
	// OpenDoor.Yaw = FMath::FInterpConstantTo( CurrentYaw, ActorRotation.Yaw + TargetYaw, DeltaTime, 45 );
	// 더 부드럽게 Interpolation 하고 싶은 경우
	OpenDoor.Yaw = FMath::FInterpTo( CurrentYaw, ActorRotation.Yaw + TargetYaw, DeltaTime, 2 );
	GetOwner()->SetActorRotation( OpenDoor );
	
	UE_LOG( LogTemp, Warning, TEXT( "Yaw is : %f" ), GetOwner()->GetActorRotation().Yaw );
}

