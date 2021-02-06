#include "launcher.h"

// Moved out of main so we can library-ify the launching for re-use.
int Launch(std::string dll) {

    TCHAR dir[MAX_PATH];
    char  dllPath[MAX_PATH];
    DWORD len = GetCurrentDirectory(MAX_PATH, &dir[0]);
    sprintf(dllPath, "%s\\%s", dir, dll.c_str());

    //sprintf(dllPath, "%s\\UE4Editor-MyMod.dll", dir);
    // void *a = LoadLibrary(dllPath);
    // printf("%llx\n", (uintptr_t)a);
    // return 0;

    // Find the running .exe
    DWORD pid = GetDungeonsProcessID();
    if (pid == 0) {
        // Attempt to launch the game.
        if (LaunchGame() == false) {
            printf("Unable to find or launch Minecraft Dungeons.\n");
            return -1;
        }

        printf("Launching Minecraft Dungeons..\n");

        // Wait until we think the game has launched and is ready for us to inject the dll.
        // TODO Maybe loop and check for loaded dlls to see if we're ready? Or something similar
        Sleep(10000);
        pid = GetDungeonsProcessID();
        if (pid == 0) {
            printf("Unable to find pid for Minecraft Dungeons\n");
            return -1;
        }
    }

    printf("Minecraft Dungeons Process ID: %d\n", pid);

    // TODO Check modules and see if our module is already loaded.
    if (HasModule(pid, dll.c_str())) {
        printf("%s already running in Minecraft Dungeons\n", dll.c_str());
        return -1;
    }

    //Open the target process with read , write and execute priviledges
    HANDLE process = OpenProcess(PROCESS_CREATE_THREAD|PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|PROCESS_VM_WRITE|PROCESS_VM_OPERATION, FALSE, pid); 
    if (process == NULL) {
        return Error(process, "Unable to open process. %d\n");
    }


    //Get the address of LoadLibraryA
    LPVOID LoadLibrary = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (LoadLibrary == NULL) {
        return Error(process, "Unable to find LoadLibrary function. %d\n");
    }

    // Allocate space in the process for our DLL 
    LPVOID memory = VirtualAllocEx(process, NULL, strlen(dllPath)+1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); 
    if (memory == NULL) {
        return Error(process, "Unable to find allocate memory in process. %d\n");
    }

    // Write the string name of our DLL in the memory allocated 
    BOOL writeResult = WriteProcessMemory(process, (LPVOID)memory, dllPath, strlen(dllPath)+1, NULL); 
    if (writeResult == FALSE) {
        return Error(process, "Unable to write memory in process. %d\n");
    }

    // Load our DLL 
    HANDLE thread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibrary, (LPVOID)memory, NULL, NULL); 
    if (thread == NULL) {
        return Error(process, "Unable to create remote thread in process. %d\n");
    }

    // Let the program regain control of itself
    BOOL closeResult = CloseHandle(process);
    if (closeResult == FALSE) {
         return Error(NULL, "Unable to close process. %d\n");
    }

    return 0;
}

int Error(HANDLE process, const char *fmt) {
    printf(fmt, GetLastError());
    if (process != NULL) {
        CloseHandle(process);
    }
    return -1;
}

// True if this process has the "module.dll" moduleName loaded. No paths
bool HasModule(const DWORD &pid, const char *moduleName) {
    bool found = false;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);

    if (snapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 curr = {0};
        curr.dwSize = sizeof(MODULEENTRY32);

        if (Module32First(snapshot, &curr)) {
            do {
                if (strcmp((const char*)curr.szModule, moduleName) == 0) {
                    found = true;
                    break;
                }
            } while (Module32Next(snapshot, &curr));
        }
        CloseHandle(snapshot);
    }
    return found;
}

// Launch any version
bool LaunchGame() {
    if (LaunchStandaloneGame()) {
        return true;
    }

    if (LaunchWinstoreGame()) {
        return true;
    }
    return false;
}

bool LaunchStandaloneGame() {

    char path[MAX_PATH];
    HRESULT hr = SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, (LPSTR)&path);
    if (!SUCCEEDED(hr)) {
        return false;
    }

    // Read launcher_settings.json for Minecraft Dungeons path
    using json = nlohmann::json;
    std::string filename = std::string(path) + "\\.minecraft_dungeons\\launcher_settings.json";
    std::ifstream in(filename);
    if (in.is_open() == false) {
        return false;
    }
    json j;
    in >> j;

    // Get the path to the .exe and attempt launching it
    std::string mcdCommand = j["productLibraryDir"].get<std::string>() + "\\dungeons\\dungeons\\Dungeons\\Binaries\\Win64\\Dungeons-Win64-Shipping.exe";

    STARTUPINFOA info={sizeof(info)};
    PROCESS_INFORMATION processInfo;

    if (CreateProcessA(NULL, (char*)mcdCommand.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo) == FALSE) {
        printf("Failed to launch standalone Minecraft Dungeons at %s\n", mcdCommand.c_str());
        return false;
    }

    return true;
}



bool LaunchWinstoreGame() {

    HINSTANCE h = ShellExecuteW(NULL, L"open", L"shell:AppsFolder\\Microsoft.Lovika_8wekyb3d8bbwe!Game", NULL, NULL, SW_SHOWNORMAL);
    if ((DWORD64)h > 32) {
        return true;
    }
    return false;
}

// Attempt to find the running .exe PID
DWORD GetDungeonsProcessID() {
    // Find them in this order. Standalone version has a Dungeons.exe running but it's the launcher exe.
    const std::vector<std::string> exeFiles = {"Dungeons-Win64-Shipping.exe", "Dungeons.exe"};
    for(const auto& exeFile: exeFiles) {
        DWORD pid = GetProcessID(exeFile.c_str());
        if (pid != 0) {
            return pid;
        }
    }
    return 0;
}

// Find process ID by exe name.
DWORD GetProcessID(const char *exeFile) {
    PROCESSENTRY32 entry;
    ZeroMemory(&entry, sizeof(entry));
    entry.dwSize = sizeof(PROCESSENTRY32);
    DWORD pid = 0;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);    
    if (Process32First(snapshot, &entry) == TRUE) {
        do {
            if (stricmp((char *)entry.szExeFile, exeFile) == 0) {
                pid = entry.th32ProcessID;
                break;
            }
        } while(Process32Next(snapshot, &entry) == TRUE);
    }

    CloseHandle(snapshot);
    return pid;
}

