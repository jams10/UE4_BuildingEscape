// Copyrights Jamin Heo 2020 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	void FindPhysicsHandle();
	void SetupInputComponent();
	void Grab();
	void Release();

	// Reach 길이 범위에 있고 Physics body를 가진 제일 처음 Actor를 리턴.
	FHitResult GetFirstPhysicsBodyInReach() const;

	// LineTraceEnd를 리턴(플레이어의 최대 Reach)
	FVector GetPlayerReach() const;

	// 월드의 플레이어의 위치를 리턴
	FVector GetPlayerWorldPosition() const;

	FRotator GetPlayerWorldRotation() const;
};
