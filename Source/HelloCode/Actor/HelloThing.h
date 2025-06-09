// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HelloThing.generated.h"

class UPickedable;

UCLASS()
class HELLOCODE_API AHelloThing : public AActor
{
	GENERATED_BODY()

public:
	AHelloThing();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ppAbleSystem")
	TObjectPtr<UPrimitiveComponent> RootMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ppAbleSystem")
	TObjectPtr<UPickedable> Pickedable;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ppAbleSystem")
	TSet<TObjectPtr<UPrimitiveComponent>> TSetAbility;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="ppAbleSystem")
	void AddDynamicAbilityComponent(const TSubclassOf<UPrimitiveComponent> AddAbilityClass);
	void RemoveDynamicAbilityComponent(const TSubclassOf<UPrimitiveComponent> RemoveAbilityClass); // 사실 비활성화만 해도 충분.. 객체소멸시 자동해제
	
public:
	virtual void Tick(const float DeltaTime) override;
};
