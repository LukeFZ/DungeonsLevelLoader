#pragma once

#include <imgui.h>
#include <string>
#include <map>

// typedef void (*OnLoadPressed)(const char *modName);
// typedef void (*OnUnloadPressed)(const char *modName);

struct UIData {
    // Json didn't like using std::pair
    std::map<std::string,std::string> seeds;
    //OnLoadPressed onLoadPressed;
    //OnUnloadPressed onUnloadPressed;
};

int StartUI(UIData *data);