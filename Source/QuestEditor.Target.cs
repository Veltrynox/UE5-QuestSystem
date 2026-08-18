using UnrealBuildTool;
using System.Collections.Generic;

public class QuestEditorTarget : TargetRules
{
	public QuestEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("QuestProject");
	}
}
