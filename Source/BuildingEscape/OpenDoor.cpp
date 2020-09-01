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
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// GetOwner를 BeginPlay가 아닌 생성자에 선언할 경우 GetOwner 호출 시 NULL이 리턴될 경우 오류가 발생할 가능성이 있으므로 주의.
	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	CurrentRotation.Yaw += 90.0f;
	GetOwner()->SetActorRotation( CurrentRotation );
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

