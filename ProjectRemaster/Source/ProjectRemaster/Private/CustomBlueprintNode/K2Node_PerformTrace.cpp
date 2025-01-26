// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomBlueprintNode/K2Node_PerformTrace.h"

#include "KismetCompiler.h"                
#include "EdGraph/EdGraphPin.h"           
#include "K2Node_CallFunction.h"          
#include "KismetCompilerModule.h"        

#include "Enums/ETraceType.h"
#include "CharacterComponents/TraceComponent.h"
#include "CharacterComponents/CustomHandPoseRecognizer.h"



void UK2Node_PerformTrace::AllocateDefaultPins()
{
    UE_LOG(LogTemp, Warning, TEXT("ExpandNode called!"));
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, TEXT("TraceComponent"), UTraceComponent::StaticClass(), TEXT("TraceComponent"));
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, TEXT("TraceType"), nullptr, TEXT("TraceType"));
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, TEXT("Vector"), nullptr, TEXT("Start"));
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, TEXT("Vector"), nullptr, TEXT("End"));
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, TEXT("Rotator"), TBaseStructure<FRotator>::Get(), TEXT("Rot"));

    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, TEXT("EOculusXRHandType"), nullptr, TEXT("HandToTrace"));
    
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, TEXT("UCustomHandPoseRecognizer"), UCustomHandPoseRecognizer::StaticClass(), TEXT("PoseClass"));

   
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Float, TEXT("ShapeRadius"), nullptr, TEXT("ShapeRadius"));

   
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Float, TEXT("HalfHeight"), nullptr, TEXT("HalfHeight"));

  
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, TEXT("DebugVisual"), nullptr, TEXT("bDebugVisual"));

    ReconstructNode();
}

FText UK2Node_PerformTrace::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return FText::FromString(TEXT("Perform Trace"));
}

void UK2Node_PerformTrace::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
    Super::ExpandNode(CompilerContext, SourceGraph);
 
  
    UEdGraphPin* TraceCompPin = FindPin(TEXT("TraceComponent"));
    UEdGraphPin* ExecPin = GetExecPin();

   
    UFunction* TargetFunction = UTraceComponent::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UTraceComponent, PerformTrace));
    if (TargetFunction)
    {
        UK2Node_CallFunction* CallFunctionNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
        CallFunctionNode->SetFromFunction(TargetFunction);
        CallFunctionNode->AllocateDefaultPins();

        
        CompilerContext.MovePinLinksToIntermediate(*TraceCompPin, *CallFunctionNode->FindPin(TEXT("TraceComponent")));
        CompilerContext.MovePinLinksToIntermediate(*ExecPin, *CallFunctionNode->GetExecPin());

        
        BreakAllNodeLinks();
    }

}

void UK2Node_PerformTrace::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
    Super::ReallocatePinsDuringReconstruction(OldPins);

   
    AllocateDefaultPins();

   
    UEdGraphPin* TraceTypePin = FindPin(TEXT("TraceType"));
    if (TraceTypePin)
    {
       
        int32 TraceTypeValue = TraceTypePin->DefaultValue.IsEmpty() ? 0 : FCString::Atoi(*TraceTypePin->DefaultValue);

      
        if (TraceTypeValue == static_cast<int32>(ETraceType::GrabTrace))
        {
            
            RemovePin(FindPin(TEXT("ShapeRadius")));
            RemovePin(FindPin(TEXT("HalfHeight")));
        }
        else if (TraceTypeValue == static_cast<int32>(ETraceType::BulletFreezeTrace))
        {
            
            RemovePin(FindPin(TEXT("PoseClass")));
            RemovePin(FindPin(TEXT("HandToTrace")));
        }
    }
}
