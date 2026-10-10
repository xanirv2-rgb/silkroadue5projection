// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SROCharacter.generated.h"

// ============================================
// HASAR TİPİ (Enum)
// ============================================
UENUM(BlueprintType)
enum class EDamageType : uint8
{
    Physical    UMETA(DisplayName = "Fiziksel"),
    Magical     UMETA(DisplayName = "Büyülü")
};

UCLASS()
class SILKROADUE_API ASROCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASROCharacter();

protected:
    virtual void BeginPlay() override;

public:	
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Hareket fonksiyonları
    void MoveForward(float Value);
    void MoveRight(float Value);
    void TurnCamera(float Value);
    void LookUpCamera(float Value);
    void StartJump();
    void StopJump();
    
    // Kamera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SRO|Camera")
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SRO|Camera")
    class UCameraComponent* FollowCamera;

    // Geçici mesh
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SRO|Mesh")
    class UStaticMeshComponent* PlaceholderMesh;

    // ============================================
    // SRO İSTATİSTİKLERİ
    // ============================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Stats")
    float Health = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Stats")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Stats")
    float Mana = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Stats")
    float MaxMana = 50.0f;

    // Saldırı
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Attack")
    float AttackPower = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Attack")
    float MagicalPower = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Attack")
    float AttackRating = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Attack")
    float CritChance = 0.20f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Attack")
    float CritMultiplier = 2.0f;

    // Defans
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Defense")
    float PhysicalDefense = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Defense")
    float MagicalDefense = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Defense")
    float Evasion = 0.10f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Defense")
    float BlockChance = 0.15f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Defense")
    float BlockReduction = 0.30f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SRO|Defense")
    float ParryChance = 0.10f;

    // Cooldown
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SRO|Cooldown")
    float GlobalCooldown = 1.5f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SRO|Cooldown")
    float LastAbilityTime = -999.0f;

    // Fonksiyonlar
    UFUNCTION(BlueprintCallable, Category = "SRO|Combat")
    void ApplyDamage(float Amount);

    UFUNCTION(BlueprintCallable, Category = "SRO|Combat")
    void Heal(float Amount);

    UFUNCTION(BlueprintPure, Category = "SRO|Combat")
    bool IsAlive() const;

    UFUNCTION(BlueprintPure, Category = "SRO|Combat")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "SRO|Combat")
    float CalculateDamage(float BaseDamage, EDamageType DamageType, AActor* Attacker);
};