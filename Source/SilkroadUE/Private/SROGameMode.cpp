// Copyright Epic Games, Inc. All Rights Reserved.

#include "SROGameMode.h"
#include "SROCharacter.h"

ASROGameMode::ASROGameMode()
{
    // Bu oyunda oyuncu SROCharacter olsun
    DefaultPawnClass = ASROCharacter::StaticClass();

    // Konsola bilgi yaz
    UE_LOG(LogTemp, Warning, TEXT("SROGameMode baslatildi - DefaultPawn: SROCharacter"));
}