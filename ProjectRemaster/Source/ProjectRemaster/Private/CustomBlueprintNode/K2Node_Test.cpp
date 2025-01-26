// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomBlueprintNode/K2Node_Test.h"

void UK2Node_Test::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();
    UE_LOG(LogTemp, Warning, TEXT("AllocateDefaultPins called for Test Node"));

    // Create a simple input pin
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, TEXT("ExecIn"));

    // Create a simple output pin
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, TEXT("ExecOut"));
}

FText UK2Node_Test::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    UE_LOG(LogTemp, Warning, TEXT("GetNodeTitle called for Test Node"));

    return FText::FromString(TEXT("Test Node"));
}
