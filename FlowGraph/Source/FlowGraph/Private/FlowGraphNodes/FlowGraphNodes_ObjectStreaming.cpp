#include "FlowGraphNodes/FlowGraphNodes_ObjectStreaming.h"
#include "FlowGraphSubsystem.h"

#if WITH_EDITOR
void UFlowGraphNode_EnableOrDisableActor::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	AddInputPin("In");
	
	AddOutputPin("Out-OperateSuccess");
	AddOutputPin("Out-ObjectNotLoaded");
}

FText UFlowGraphNode_EnableOrDisableActor::GetNodeDisplayName() const
{
	return NSLOCTEXT("FlowGraphEditor", "Enable or Disable Actor DisplayName", "启用或禁用Actor");
}

FText UFlowGraphNode_EnableOrDisableActor::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (bEnable)
	{
		return NSLOCTEXT("FlowGraphEditor", "Enable or Disable Actor Node Title - Enable", "启用Actor");
	}
	return NSLOCTEXT("FlowGraphEditor", "Enable or Disable Actor Node Title - Disable", "禁用Actor");
}

void UFlowGraphNode_EnableOrDisableActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_CHECKED(ThisClass, bEnable))
	{
		GetGraph()->NotifyNodeChanged(this);
	}
}

FLinearColor UFlowGraphNode_EnableOrDisableActor::GetNodeTitleColor() const
{
	return FLinearColor::Yellow;
}

FText UFlowGraphNode_EnableOrDisableActor::GetTooltipText() const
{
	return NSLOCTEXT("FlowGraphEditor", "Enable or Disable Actor Node Tooltip", "启用或禁用一个Actor，内部含有一个软指针，"
																				"软指针允许填写可能在运行时未被加载的对象");
}

FText UFlowGraphNode_EnableOrDisableActor::GetNodeCategory() const
{
	return FlowGraphObjectStreamingCategory();
}

void UFlowGraphNode_SpawnActorAt::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
	AddInputPin("In");
	AddOutputPin("Out");
}

FText UFlowGraphNode_SpawnActorAt::GetNodeDisplayName() const
{
	return NSLOCTEXT("FlowGraphEditor", "Spawn Actor Node Display Name", "生成Actor");
}

FLinearColor UFlowGraphNode_SpawnActorAt::GetNodeTitleColor() const
{
	return FLinearColor::Transparent;
}

FText UFlowGraphNode_SpawnActorAt::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FText Title = NSLOCTEXT("FlowGraphEditor", "Spawn Actor Node Title - nullptr", "生成Actor");
	if (ActorClassToSpawn == nullptr)
	{
		return Title;
	}
	return FText::FromString(Title.ToString() + ActorClassToSpawn.Get()->GetName());
}

void UFlowGraphNode_SpawnActorAt::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_CHECKED(ThisClass, ActorClassToSpawn))
	{
		GetGraph()->NotifyNodeChanged(this);
		OnNodeCancelSelected();
		OnNodeSelected();
	}
}

void UFlowGraphNode_SpawnActorAt::OnNodeSelected()
{
	Super::OnNodeSelected();

	if (ActorClassToSpawn != nullptr)
	{
		UWorld* PIEWorld = nullptr;
		for (FWorldContext const& Context : GEngine->GetWorldContexts())
		{
			if (Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::GamePreview || Context.WorldType == EWorldType::Editor)
			{
				PIEWorld = Context.World();
				break;
			}
		}

		if (PIEWorld == nullptr)
		{
			UE_LOG(LogFlowGraph, Warning, TEXT("Spawn Actor Node Cannot find PIE world so it world not generate preview avatar"))
			return;
		}
		
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ActorAvatar = PIEWorld->SpawnActor<AActor>(ActorClassToSpawn, SpawnTransform, ActorSpawnParameters);
	}
}

void UFlowGraphNode_SpawnActorAt::OnNodeCancelSelected()
{
	Super::OnNodeCancelSelected();

	if (ActorAvatar != nullptr)
	{
		SpawnTransform = ActorAvatar->GetActorTransform();
		ActorAvatar->Destroy();
	}
}

FText UFlowGraphNode_SpawnActorAt::GetTooltipText() const
{
	return NSLOCTEXT("FlowGraphEditor", "Spawn Actor Node Tooltip", "在指定变换下生成Actor");
}

FText UFlowGraphNode_SpawnActorAt::GetNodeCategory() const
{
	return FlowGraphObjectStreamingCategory();
}
#endif

void UFlowGraphNode_EnableOrDisableActor::OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator)
{
	if (!ActorToModify.IsValid())
	{
		if (bLoadActorIfPointerNotValid)
		{
			ActorToModify = ActorToModify.LoadSynchronous();
		}
		else
		{
			TriggerOutput(InIterator, "Out-ObjectNotLoaded");
			return;
		}
	}

	if (bEnable)
	{
		ActorToModify->SetActorEnableCollision(true);
		ActorToModify->SetActorHiddenInGame(false);
		ActorToModify->SetActorTickEnabled(true);
		ActorToModify->SetCanBeDamaged(true);
	
		if (APawn* Pawn = Cast<APawn>(ActorToModify.Get()))
		{
			Pawn->DisableInput(nullptr);
		}
	}
	else
	{
		ActorToModify->SetActorEnableCollision(false);
		ActorToModify->SetActorHiddenInGame(true);
		ActorToModify->SetActorTickEnabled(false);
		ActorToModify->SetCanBeDamaged(false);
	
		if (APawn* Pawn = Cast<APawn>(ActorToModify.Get()))
		{
			Pawn->DisableInput(nullptr);
		}
	}
	TriggerOutput(InIterator, "Out-OperateSuccess");
}

void UFlowGraphNode_SpawnActorAt::OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator)
{
	if (ActorClassToSpawn != nullptr)
	{
		GWorld->SpawnActor<AActor>(ActorClassToSpawn.Get(), SpawnTransform);
	}
	TriggerOutput(InIterator, "Out");
}
