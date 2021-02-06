#pragma once

#include <imgui.h>

// Tick,Blueprint,ReceiveDraw,EvaluateGraph,IsPointHidden,SetRenderOp,RetryEvent,Audio,UpdateFadeActors,InpActEvt,Timeline_0__UpdateFunc,SwampBubbles_C,OnActorBump,ReceiveHit,OnActorHit,Overlap,Capsule

typedef void (*TextCallback)(char* data);

struct UIData {
//    TextCallback filterExcludeCallback;
//    TextCallback filterIncludeCallback;
    char *filterExclude = nullptr;
    char *filterInclude = nullptr;
    bool showActorEvents = true;
    bool showObjectEvents = false;
    bool showObjectProperties = false;
    bool showFunctionParameters = false;
    bool filterExcludeEnabled = true;
    bool filterIncludeEnabled = false;
    int propertyMaxLevel = 2;
};

int StartUI(UIData* textEditCallback);