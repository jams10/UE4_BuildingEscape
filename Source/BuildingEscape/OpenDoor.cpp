// Copyrights Jamin Heo 2020 

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFrameWork/PlayerController.h"
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
// GetOwner�� BeginPlay�� �ƴ� �����ڿ� ������ ��� GetOwner ȣ�� �� NULL�� ���ϵ� ��� ������ �߻��� ���ɼ��� �����Ƿ� ����.
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	if( !PressurePlate )
	{
		UE_LOG( LogTemp, Error, TEXT( "%s has OpenDoor Component, but has no Pressure plate." ), *GetOwner()->GetName() );
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// if(ptr) : �����Ͱ� NULL ���������� ���θ� �����ϰ� üũ�� �� ����. null�� �ƴ� ��� ������ ����.
	if( TotalMassOfActors() >= MassToOpenDoor )
	{ 
		OpenDoor( DeltaTime );
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if( GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay )
		{
			CloseDoor( DeltaTime );
		}
	}
}

void UOpenDoor::OpenDoor( float DeltaTime )
{
	//CurrentYaw = FMath::FInterpTo( CurrentYaw, OpenAngle, DeltaTime, 3 );
	CurrentYaw = FMath::Lerp( CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed );
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation( DoorRotation );

	//UE_LOG( LogTemp, Warning, TEXT( "Yaw is : %f" ), GetOwner()->GetActorRotation().Yaw );
}

void UOpenDoor::CloseDoor( float DeltaTime )
{
	// Lerp �Լ����� ����° �Ű������� Alpha ���� 1�� ����� ���� �ι�° �Ű����� ���� �� ���� ���� ��.
	// ���� Alpha ���� Ŭ ���� ���� �� ���� ������ ��.
	CurrentYaw = FMath::Lerp( CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed );
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation( DoorRotation );
}

// Frame�� ���������� ����. ��� ���� ��ŭ interpolate 0�� : 0�� -> 1�� : 45�� -> 2�� : 90�� (Linear Interpolation)
// OpenDoor.Yaw = FMath::FInterpConstantTo( CurrentYaw, ActorRotation.Yaw + OpenAngle, DeltaTime, 45 );
// �� �ε巴�� Interpolation �ϰ� ���� ���
// CurrentYaw = FMath::FInterpTo( CurrentYaw, OpenAngle, DeltaTime, 2 );

// ���� PC ���� DeltaTime �� : 0.008334

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;

	if( !PressurePlate ) return;
	PressurePlate->GetOverlappingActors( OUT OverlappingActors );

	for( AActor* Actor : OverlappingActors )
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

