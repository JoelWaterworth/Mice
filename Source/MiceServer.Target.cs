using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class MiceServerTarget : TargetRules
{
    public MiceServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        bUsesSteam = false;

        ExtraModuleNames.Add("Mice");
    }
}
