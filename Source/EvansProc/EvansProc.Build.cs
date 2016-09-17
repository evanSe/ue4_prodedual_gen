// Some copyright should be here...

using UnrealBuildTool;

public class EvansProc : ModuleRules
{
	public EvansProc(TargetInfo Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"EvansProc/Public"
				
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"EvansProc/Private",
				
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"ProceduralMeshComponent",

				// ... add private dependencies that you statically link with here ...	
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
