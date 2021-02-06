#include <diet-ue.h>
#include <helpers.h>
#include <Misc/DefaultValueHelper.h>


namespace autocomplete {
    IConsoleManager *ConsoleManager = nullptr;
    //UConsoleSettings *ConsoleSettings = nullptr;

}

UClass *UConsoleSettings::GetPrivateStaticClass(void) {
    static UClass* cls = (UClass*)spy::FindObjectByName("ConsoleSettings", "Class");
    return cls;
};


UClass *UFunction::GetPrivateStaticClass(void) {
    static UClass* cls = (UClass*)spy::FindObjectByName("Function", "Class");
    return cls;
};

class FConsoleVariableAutoCompleteVisitor 
{
public:
	// @param Name must not be 0
	// @param CVar must not be 0
	static void OnConsoleVariable(const TCHAR *Name, IConsoleObject* CVar,TArray<struct FAutoCompleteCommand>& Sink)
	{

		// if(CVar->TestFlags(ECVF_Cheat))
		// {
		// 	return;
		// }

		// if(CVar->TestFlags(ECVF_Unregistered))
		// {
		// 	return;
		// }

        const UConsoleSettings* ConsoleSettings = GetDefault<UConsoleSettings>();

		// can be optimized
		int32 NewIdx = Sink.AddDefaulted();
		FAutoCompleteCommand& Cmd = Sink[NewIdx];
		Cmd.Command = Name;

		IConsoleVariable* CVariable = CVar->AsVariable();
		if (CVariable)
		{
			if (CVar->TestFlags(ECVF_ReadOnly))
			{
				Cmd.Color = ConsoleSettings->AutoCompleteFadedColor;
			}
			else
			{
				Cmd.Color = ConsoleSettings->AutoCompleteCVarColor;
			}
		}
		else
		{
			Cmd.Color = ConsoleSettings->AutoCompleteCommandColor;
		}
	}
};


class SpyConsole : public UConsole {
public:

    void SpyBuildRuntimeAutoCompleteList(bool bForce = false)
    {
        if (!bForce)
        {
            // unless forced delay updating until needed
            bIsRuntimeAutoCompleteUpToDate = false;
            return;
        }

        // clear the existing tree
        //@todo - probably only need to rebuild the tree + partial command list on level load
        for (int32 Idx = 0; Idx < AutoCompleteTree.ChildNodes.Num(); Idx++)
        {
            FAutoCompleteNode *Node = AutoCompleteTree.ChildNodes[Idx];
            delete Node;
        }

        AutoCompleteTree.ChildNodes.Reset();

        // Private member..
        const UConsoleSettings* ConsoleSettings = GetDefault<UConsoleSettings>();

        // copy the manual list first
        AutoCompleteList.Reset();
        AutoCompleteList.AddDefaulted(ConsoleSettings->ManualAutoCompleteList.Num());
        for (int32 Idx = 0; Idx < ConsoleSettings->ManualAutoCompleteList.Num(); Idx++)
        {
            AutoCompleteList[Idx] = ConsoleSettings->ManualAutoCompleteList[Idx];
            AutoCompleteList[Idx].Color = ConsoleSettings->AutoCompleteCommandColor;
        }

        // systems that have registered to want to introduce entries
        // RegisterConsoleAutoCompleteEntries.Broadcast(AutoCompleteList);

        // console variables
        {
            autocomplete::ConsoleManager->ForEachConsoleObjectThatStartsWith(
                FConsoleObjectVisitor::CreateStatic< TArray<struct FAutoCompleteCommand>& >(
                    &FConsoleVariableAutoCompleteVisitor::OnConsoleVariable,
                    AutoCompleteList ) );
        }

    // 	// iterate through script exec functions and append to the list
    // 	for (TObjectIterator<UFunction> It; It; ++It)
    //	{
    // 		UFunction *Func = *It;

    // 		// Determine whether or not this is a level script event that we can call (must be defined in the level script actor and not in parent, and has no return value)
    // 		const UClass* FuncOuter = Cast<UClass>(Func->GetOuter());
    // 		const bool bIsLevelScriptFunction = FuncOuter 
    // //			&& (FuncOuter->IsChildOf(ALevelScriptActor::StaticClass()))
    // //			&& (FuncOuter != ALevelScriptActor::StaticClass())
    // 			&& (Func->ReturnValueOffset == MAX_uint16) 
    // 			&& (Func->GetSuperFunction() == nullptr);

    // 		// exec functions that either have no parent, level script events, or are in the global state (filtering some unnecessary dupes)
    // 		if ( (Func->HasAnyFunctionFlags(FUNC_Exec) && (Func->GetSuperFunction() == nullptr || FuncOuter))
    // 			|| bIsLevelScriptFunction)
    // 		{
    // 			FString FuncName = Func->GetName();
    // 			if(FDefaultValueHelper::HasWhitespaces(FuncName))
    // 			{
    // 				FuncName = FString::Printf(TEXT("\"%s\""), *FuncName);
    // 			}
    // 			if( bIsLevelScriptFunction )
    // 			{
    // 				FuncName = FString(TEXT("ce ")) + FuncName;
    // 			}

    // 			int32 Idx = 0;
    // 			for (; Idx < AutoCompleteList.Num(); ++Idx)
    // 			{
    // 				if (AutoCompleteList[Idx].Command.ToLower() == FuncName)
    // 				{
    // 					break;
    // 				}
    // 			}

    // 			const int32 NewIdx = (Idx < AutoCompleteList.Num()) ? Idx : AutoCompleteList.AddDefaulted();
    // 			AutoCompleteList[NewIdx].Command = FuncName;
    // 			AutoCompleteList[NewIdx].Color = ConsoleSettings->AutoCompleteCommandColor;

    // 			FString Desc;

    // 			// build a help string
    // 			// append each property (and it's type) to the help string
    // 			for (TFieldIterator<UProperty> PropIt(Func); PropIt && (PropIt->PropertyFlags & CPF_Parm); ++PropIt)
    // 			{
    // 				UProperty *Prop = *PropIt;
    // 				Desc += FString::Printf(TEXT("%s[%s] "),*Prop->GetName(),*Prop->GetCPPType());
    // 			}
    // 			AutoCompleteList[NewIdx].Desc = Desc + AutoCompleteList[NewIdx].Desc;
    // 		}
    //	}

        // // enumerate maps
        // {
        // 	TArray<FString> Packages;
        // 	for (int32 PathIdx = 0; PathIdx < ConsoleSettings->AutoCompleteMapPaths.Num(); ++PathIdx)
        // 	{
        // 		FPackageName::FindPackagesInDirectory(Packages, FString::Printf(TEXT("%s%s"), *FPaths::ProjectDir(), *ConsoleSettings->AutoCompleteMapPaths[PathIdx]));
        // 	}
        
        // 	// also include maps in this user's developer dir
        // 	FPackageName::FindPackagesInDirectory(Packages, FPaths::GameUserDeveloperDir());

        // 	for (int32 PackageIndex = 0; PackageIndex < Packages.Num(); PackageIndex++)
        // 	{
        // 		FString Pkg = MoveTemp(Packages[PackageIndex]);
        // 		const int32 ExtIdx = Pkg.Find(*FPackageName::GetMapPackageExtension(), ESearchCase::IgnoreCase, ESearchDir::FromEnd);

        // 		FString MapName;
        // 		if (ExtIdx != INDEX_NONE && Pkg.Split(TEXT("/"), nullptr, &MapName, ESearchCase::CaseSensitive, ESearchDir::FromEnd))
        // 		{
        // 			// try to peel off the extension
        // 			FString TrimmedMapName;
        // 			if (!MapName.Split(TEXT("."), &TrimmedMapName, nullptr, ESearchCase::CaseSensitive, ESearchDir::FromEnd))
        // 			{
        // 				TrimmedMapName = MapName;
        // 			}
        // 			int32 NewIdx;
        // 			// put _P maps at the front so that they match early, since those are generally the maps we want to actually open
        // 			if (TrimmedMapName.EndsWith(TEXT("_P")))
        // 			{
        // 				NewIdx = 0;
        // 				AutoCompleteList.InsertDefaulted(0, 3);
        // 			}
        // 			else
        // 			{
        // 				NewIdx = AutoCompleteList.AddDefaulted(3);
        // 			}
        // 			AutoCompleteList[NewIdx].Command = FString::Printf(TEXT("open %s"), *TrimmedMapName);
        // 			AutoCompleteList[NewIdx].Color = ConsoleSettings->AutoCompleteCommandColor;
        // 			AutoCompleteList[NewIdx + 1].Command = FString::Printf(TEXT("travel %s"), *TrimmedMapName);
        // 			AutoCompleteList[NewIdx + 1].Color = ConsoleSettings->AutoCompleteCommandColor;
        // 			AutoCompleteList[NewIdx + 2].Command = FString::Printf(TEXT("servertravel %s"), *TrimmedMapName);
        // 			AutoCompleteList[NewIdx + 2].Color = ConsoleSettings->AutoCompleteCommandColor;
        // 		}
        // 	}
        // }

        // // misc commands
        // {
        // 	const int32 NewIdx = AutoCompleteList.AddDefaulted();
        // 	AutoCompleteList[NewIdx].Command = FString(TEXT("open 127.0.0.1"));
        // 	AutoCompleteList[NewIdx].Desc = FString(TEXT("(opens connection to localhost)"));
        // 	AutoCompleteList[NewIdx].Color = ConsoleSettings->AutoCompleteCommandColor;
        // }

        // // Add all showflag commands.
        // {
        // 	struct FIterSink
        // 	{
        // 		FIterSink(TArray<FAutoCompleteCommand>& InAutoCompleteList)
        // 			: AutoCompleteList(InAutoCompleteList)
        // 		{
        // 		}
                
        // 		bool OnEngineShowFlag(uint32 InIndex, const FString& InName)
        // 		{
        // 			// Get localized name.
        // 			FText LocName;
        // 			FEngineShowFlags::FindShowFlagDisplayName(InName, LocName);
                    
        // 			int32 NewIdx = AutoCompleteList.AddDefaulted();
        // 			AutoCompleteList[NewIdx].Command = TEXT("show ") + InName;
        // 			AutoCompleteList[NewIdx].Desc = FString::Printf(TEXT("(toggles the %s showflag)"),*LocName.ToString());
        // 			AutoCompleteList[NewIdx].Color = GetDefault<UConsoleSettings>()->AutoCompleteCommandColor;
                    
        // 			return true;
        // 		}
                
        // 		TArray<FAutoCompleteCommand>& AutoCompleteList;
        // 	};
            
        // 	FIterSink Sink(AutoCompleteList);
        // 	FEngineShowFlags::IterateAllFlags(Sink);
        // }

        // Add any commands from UConsole subclasses
        // AugmentRuntimeAutoCompleteList(AutoCompleteList);

        AutoCompleteList.Shrink();

        // build the magic tree!
        for (int32 ListIdx = 0; ListIdx < AutoCompleteList.Num(); ListIdx++)
        {
            FString Command = AutoCompleteList[ListIdx].Command.ToLower();
            FAutoCompleteNode *Node = &AutoCompleteTree;
            for (int32 Depth = 0; Depth < Command.Len(); Depth++)
            {
                int32 Char = Command[Depth];
                int32 FoundNodeIdx = INDEX_NONE;
                TArray<FAutoCompleteNode*> &NodeList = Node->ChildNodes;
                for (int32 NodeIdx = 0; NodeIdx < NodeList.Num(); NodeIdx++)
                {
                    if (NodeList[NodeIdx]->IndexChar == Char)
                    {
                        FoundNodeIdx = NodeIdx;
                        Node = NodeList[FoundNodeIdx];
                        NodeList[FoundNodeIdx]->AutoCompleteListIndices.Add(ListIdx);
                        break;
                    }
                }
                if (FoundNodeIdx == INDEX_NONE)
                {
                    FAutoCompleteNode *NewNode = new FAutoCompleteNode(Char);
                    NewNode->AutoCompleteListIndices.Add(ListIdx);
                    Node->ChildNodes.Add(NewNode);
                    Node = NewNode;
                }
            }
        }
        bIsRuntimeAutoCompleteUpToDate = true;
        //PrintNode(&AutoCompleteTree);

    }
};


namespace autocomplete {
   
    void BuildAutocomplete(IConsoleManager *ConsoleManager, UConsole *Console) {
        autocomplete::ConsoleManager = ConsoleManager;
        SpyConsole *con = (SpyConsole*)Console;
        con->SpyBuildRuntimeAutoCompleteList(true);
    }
}

// bool FDefaultValueHelper::HasWhitespaces(const FString& Source)
// {
// 	for(int Pos = 0; Pos < Source.Len(); ++Pos)
// 	{
// 		if( IsWhitespace( Source[Pos] ) )
// 		{
// 			return true;
// 		}
// 	}
// 	return false;
// }
