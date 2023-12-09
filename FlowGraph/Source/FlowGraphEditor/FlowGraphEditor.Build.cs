using UnrealBuildTool;

public class FlowGraphEditor : ModuleRules
{
    public FlowGraphEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "UnrealEd",
                "Slate",
                "SlateCore",
                "FlowGraph",
                "AssetDefinition",
                "ToolMenus",
                "AssetTools", 
            }
        );
    }
}