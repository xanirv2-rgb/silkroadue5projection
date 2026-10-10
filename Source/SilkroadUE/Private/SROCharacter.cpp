// Copyright Epic Games, Inc. All Rights Reserved.

#include "SROCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"

// ============================================
// CONSTRUCTOR
// ============================================
ASROCharacter::ASROCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Kamera
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

    // Geçici mesh
    PlaceholderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaceholderMesh"));
    PlaceholderMesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        PlaceholderMesh->SetStaticMesh(CubeMesh.Object);
        PlaceholderMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
    }
}

// ============================================
// BEGIN PLAY
// ============================================
void ASROCharacter::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("SROCharacter BeginPlay: %s - Health: %.1f / %.1f"),
        *GetName(), Health, MaxHealth);
}

// ============================================
// TICK
// ============================================
void ASROCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// ============================================
// INPUT
// ============================================
void ASROCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (PlayerInputComponent)
    {
        // Test aksiyonları
        PlayerInputComponent->BindAction("Damage", IE_Pressed, this, &ASROCharacter::OnDamagePressed);
        PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &ASROCharacter::OnHealPressed);

        // Hareket (WASD)
        PlayerInputComponent->BindAxis("MoveForward", this, &ASROCharacter::MoveForward);
        PlayerInputComponent->BindAxis("MoveRight", this, &ASROCharacter::MoveRight);

        // Kamera (Mouse)
        PlayerInputComponent->BindAxis("Turn", this, &ASROCharacter::TurnCamera);
        PlayerInputComponent->BindAxis("LookUp", this, &ASROCharacter::LookUpCamera);

        // Zıplama
        PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASROCharacter::StartJump);
        PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASROCharacter::StopJump);
    }
}

// ============================================
// APPLY DAMAGE
// ============================================
void ASROCharacter::ApplyDamage(float Amount)
{
    // OLU KONTROLU: Ölü karakter hasar almaz
    if (!IsAlive())
    {
        UE_LOG(LogTemp, Warning, TEXT("%s OLU - hasar almaz!"), *GetName());
        return;
    }

    float FinalDamage = CalculateDamage(Amount, EDamageType::Physical, nullptr);

    if (FinalDamage <= 0.0f)
    {
        return;
    }

    Health -= FinalDamage;
    if (Health < 0.0f)
    {
        Health = 0.0f;
    }

    UE_LOG(LogTemp, Warning, TEXT("%s hasar aldi: %.1f - Kalan can: %.1f / %.1f"),
        *GetName(), FinalDamage, Health, MaxHealth);

    if (!IsAlive())
    {
        UE_LOG(LogTemp, Error, TEXT("%s OLDU!"), *GetName());
    }
}

// ============================================
// CALCULATE DAMAGE
// ============================================
float ASROCharacter::CalculateDamage(float BaseDamage, EDamageType DamageType, AActor* Attacker)
{
    // 1. Dodge
    if (FMath::FRand() < Evasion)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] DODGE! Kacindi."), *GetName());
        return 0.0f;
    }

    // 2. Parry
    if (FMath::FRand() < ParryChance)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] PARRY! Silahtan reddetti."), *GetName());
        return 0.0f;
    }

    // 3. Block
    bool bBlocked = false;
    if (FMath::FRand() < BlockChance)
    {
        bBlocked = true;
        UE_LOG(LogTemp, Warning, TEXT("[%s] BLOCK! Kalkanla emdi."), *GetName());
    }

    // 4. Defans formulu (Silkroad)
    float Damage = BaseDamage;
    float DefenseValue = (DamageType == EDamageType::Physical) ? PhysicalDefense : MagicalDefense;
    Damage = Damage * (100.0f / (100.0f + DefenseValue));

    // Block azaltma
    if (bBlocked)
    {
        Damage = Damage * (1.0f - BlockReduction);
    }

    // 5. Critical
    if (FMath::FRand() < CritChance)
    {
        Damage = Damage * CritMultiplier;
        UE_LOG(LogTemp, Warning, TEXT("[%s] CRITICAL! %.1f hasar!"), *GetName(), Damage);
    }

    return Damage;
}

// ============================================
// HEAL
// ============================================
void ASROCharacter::Heal(float Amount)
{
    // OLU KONTROLU: Ölü karakter iyileşemez
    if (!IsAlive())
    {
        UE_LOG(LogTemp, Warning, TEXT("%s OLU - iyilestirilemez!"), *GetName());
        return;
    }

    Health += Amount;
    if (Health > MaxHealth)
    {
        Health = MaxHealth;
    }

    UE_LOG(LogTemp, Warning, TEXT("%s iyilesti: %.1f - Can: %.1f / %.1f"),
        *GetName(), Amount, Health, MaxHealth);
}

// ============================================
// IS ALIVE
// ============================================
bool ASROCharacter::IsAlive() const
{
    return Health > 0.0f;
}

// ============================================
// GET HEALTH PERCENT
// ============================================
float ASROCharacter::GetHealthPercent() const
{
    if (MaxHealth <= 0.0f)
    {
        return 0.0f;
    }
    return Health / MaxHealth;
}
// ============================================
// HAREKET FONKSİYONLARI
// ============================================

// WASD - İleri/Geri
void ASROCharacter::MoveForward(float Value)
{
    if (Value != 0.0f && Controller)
    {
        // Kameranın baktığı yönü al
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // İleri yön vektörünü hesapla
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        
        // Karakteri hareket ettir
        AddMovementInput(Direction, Value);
    }
}

// A/D - Sağa/Sola
void ASROCharacter::MoveRight(float Value)
{
    if (Value != 0.0f && Controller)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Sağ yön vektörünü hesapla
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // Karakteri hareket ettir
        AddMovementInput(Direction, Value);
    }
}

// Mouse X - Kamera sağa/sola dön
void ASROCharacter::TurnCamera(float Value)
{
    if (Value != 0.0f)
    {
        AddControllerYawInput(Value);
    }
}

// Mouse Y - Kamera yukarı/aşağı bak
void ASROCharacter::LookUpCamera(float Value)
{
    if (Value != 0.0f)
    {
        AddControllerPitchInput(Value);
    }
}

// Space - Zıplama başlat
void ASROCharacter::StartJump()
{
    Jump();
}

// Space bırak - Zıplama bitir
void ASROCharacter::StopJump()
{
    StopJumping();
}