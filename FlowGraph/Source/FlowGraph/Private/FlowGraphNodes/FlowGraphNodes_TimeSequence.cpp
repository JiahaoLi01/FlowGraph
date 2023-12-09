#include "FlowGraphNodes/FlowGraphNodes_TimeSequence.h"
#include "FlowGraphNodeIterator.h"
#include "FlowGraphSubsystem.h"
#include "FlowGraph_Graph.h"

#if WITH_EDITOR
void UFlowGraphNode_Begin::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	AddOutputPin("Output");
}

FLinearColor UFlowGraphNode_Begin::GetNodeTitleColor() const
{
	return FLinearColor::Red;
}

FText UFlowGraphNode_Begin::GetTooltipText() const
{
	return NSLOCTEXT("FlowGraphEditor", "Begin Node Tooltip", "根节点，当一张图被注册到流图子系统时，会从此处自动创建一个图迭代器并立即开始工作,"
															  "该类的子类也是同样的逻辑。");
}

FText UFlowGraphNode_Begin::GetNodeDisplayName() const
{
	return NSLOCTEXT("FlowGraphEditor", "Begin Node Title", "流图根节点");
}

FText UFlowGraphNode_Begin::GetNodeCategory() const
{
	return FlowGraphTimeSequenceCategory();
}

void UFlowGraphNode_End::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	AddInputPin("In");
}

FLinearColor UFlowGraphNode_End::GetNodeTitleColor() const
{
	return FLinearColor::Blue;
}

FText UFlowGraphNode_End::GetTooltipText() const
{
	return NSLOCTEXT("FlowGraphEditor", "End Node Tooltip", "结束节点，会从子系统中注销子图，该节点会停止所有正在工作的迭代器的工作,"
															  "该类的子类也是同样的逻辑。");
}

FText UFlowGraphNode_End::GetNodeDisplayName() const
{
	return NSLOCTEXT("FlowGraphEditor", "End Node Title", "结束并取消所有迭代器");
}

FText UFlowGraphNode_End::GetNodeCategory() const
{
	return FlowGraphTimeSequenceCategory();
}

#endif

void UFlowGraphNode_Begin::OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator)
{
	Super::OnNodeIteratorIn_Implementation(InIterator);
	TriggerOutput(InIterator, "Output");
}

void UFlowGraphNode_End::OnNodeIteratorIn_Implementation(UFlowGraphNodeIterator* InIterator)
{
	Super::OnNodeIteratorIn_Implementation(InIterator);

	InIterator->IteratorTo(nullptr);
	const UFlowGraph_Graph* OwnerGraph = Cast<UFlowGraph_Graph>(GetGraph());

	if (UFlowGraphSubsystem* FlowGraphSubsystem = OwnerGraph->GetFlowGraph()->FlowGraphSubsystem.Get())
	{
		FlowGraphSubsystem->UnregisterFlowGraph(OwnerGraph->GetFlowGraph());
	}
}
