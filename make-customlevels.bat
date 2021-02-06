
cl /nologo /EHsc ^
     /I launcher/ ^
     customlevels\main.cpp ^
     launcher\launcher.cpp ^
     -link^
      /subsystem:console -OUT:customlevels.exe

cl /nologo /Zi /EHsc ^
     /I minhook-master/include/ /I unrealspy/ ^
     /I imgui-1.78/  ^
     /I "%WindowsSdkDir%Include\um" /I "%WindowsSdkDir%Include\shared"  ^
     /FI M:\DevEnvironment\DevWorkspace\CLionProjects\dungeoneer-master\diet-ue\defines.h ^
     /I diet-ue/ ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\Core\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\Core\Private" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\CoreUObject\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\Engine\Classes" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\Engine\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\EngineSettings\Classes" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\InputCore\Classes" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\RHI\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\RenderCore\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\SlateCore\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\AssetRegistry\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\ApplicationCore\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\Renderer\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\Slate\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\AudioMixer\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\AudioPlatformConfiguration\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\AnimationCore\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\Sockets\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\PacketHandlers\PacketHandler\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\NetworkReplayStreaming\NetworkReplayStreaming\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\Json\Public" ^
     /I "M:\EpicGames\UE_4.22\Engine\Source\Runtime\Public\GenericPlatform" ^
     /I "M:\EpicGames\UE_4.22\Engine\Intermediate\Build\Win64\UE4\Inc\Engine" ^
     /I "M:\EpicGames\UE_4.22\Engine\Intermediate\Build\Win64\UE4\Inc\InputCore" ^
     /I "M:\EpicGames\UE_4.22\Engine\Intermediate\Build\Win64\UE4\Inc\EngineSettings" ^
     /I "M:\EpicGames\UE_4.22\Engine\Intermediate\Build\Win64\UE4\Inc\SlateCore" ^
     /I "M:\EpicGames\UE_4.22\Engine\Intermediate\Build\Win64\UE4\Inc\AssetRegistry" ^
     /I "M:\EpicGames\UE_4.22\Engine\Intermediate\Build\Win64\UE4\Inc\Slate" ^
     /I "M:\EpicGames\UE_4.22\Engine\Intermediate\Build\Win64\UE4\Inc\AudioPlatformConfiguration" ^
     customlevels\dll.cpp customlevels\ui.cpp ^
     unrealspy\*.cpp ^
     diet-ue\*.cpp ^
     minhook-master\src\hook.c^
     minhook-master\src\buffer.c^
     minhook-master\src\trampoline.c^
     minhook-master\src\hde\hde64.c^
     imgui-1.78\imgui*.cpp^
     customlevels\imgui\*.cpp ^
     -link ^
      d3d11.lib d3dcompiler.lib ^
      -DLL /subsystem:console -OUT:customlevels.dll
