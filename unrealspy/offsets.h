#pragma once

#include <string>
#include <vector>
#include <map>
#include <unrealspy.h>
#include <Windows/AllowWindowsPlatformTypes.h>
	#include <windows.h>
    #include <TlHelp32.h>
#include <Windows/HideWindowsPlatformTypes.h>

namespace offsets {

    // ["name"] = opcodes string
    extern std::map<UE4Reference,std::string> defaultAddressLookups;

    std::map<UE4Reference,uintptr_t> FindAddresses(HANDLE process, std::map<UE4Reference,std::string> lookups);
    std::map<UE4Reference,uintptr_t> FindAddresses(HANDLE process, MODULEENTRY32 modEntry, std::map<UE4Reference, std::string> opcodes);

    //uintptr_t FindOffset(HANDLE process, MODULEENTRY32 modEntry, std::string opcodes);
    MODULEENTRY32 GetModule(const DWORD &pid);
    size_t parseHex(std::string hex, BYTE *bytes, char *mask);


    // Taken from https://github.com/0xZ0F/CPPMemory/blob/master/x64/Scanning.cpp

    // Actual pattern scanning:
    uintptr_t PatternScan(char* bytes, size_t size, const BYTE* pattern, const char* mask, size_t patternLen);
    
    // TODO This MemoryIterator is very specific to our pattern matching.
    // Not clear from the outside that it does some magic shuffling of the 128 bytes.
    class MemoryIterator {

    private:
        // 128 bytes so we can match across CHUNKS
        static const int PATTERN_MAX_SIZE = 128; // 128 crashes! But didn't use to. Maybe just double buffer size

        static const int CHUNK_SIZE = 4096;
        HANDLE hProc;
        char buffer[CHUNK_SIZE+PATTERN_MAX_SIZE];
        SIZE_T bytesRead = 0;

        uintptr_t current;
        uintptr_t end;

    public:
        MemoryIterator(const HANDLE hProc, uintptr_t begin, uintptr_t end) : hProc(hProc), current(begin), end(end)
        {
            ReadData();
        }

        /** conversion to "bool" returning true if the iterator is valid. */
        inline explicit operator bool() const
        { 
            return hProc != NULL; 
        }
        /** inverse of the "bool" operator */
        inline bool operator !() const 
        {
            return !(bool)*this;
        }

        inline friend bool operator==(const MemoryIterator& Lhs, const MemoryIterator& Rhs) { return Lhs.hProc == Rhs.hProc; }
        inline friend bool operator!=(const MemoryIterator& Lhs, const MemoryIterator& Rhs) { return Lhs.hProc != Rhs.hProc; }

        inline void operator++() {
            ReadData();
        }

        inline char* operator*(){
            return (char*)&buffer[0];
        }

        inline char* operator->() {
            return (char*)&buffer[0];
        }

        inline SIZE_T GetBytesRead() {
            return bytesRead;
        }

        inline uintptr_t GetCurrentAddr() {
            return current - PATTERN_MAX_SIZE;
        }

    protected:
        inline void ReadData() {
            current = current + bytesRead; // 0 first time anyway
            if (current >= end) {
                hProc = NULL;
                return;
            }

            // Copy last iteration's last 128 bytes to start of buffer to find patterns crossing 4096 border
            memcpy(&buffer[0], &buffer[CHUNK_SIZE], PATTERN_MAX_SIZE);

            DWORD oldProtect;
            VirtualProtectEx(hProc, (void*)current, CHUNK_SIZE, PAGE_EXECUTE_READWRITE, &oldProtect);
            // Write to &buffer[128]
            ReadProcessMemory(hProc, (void*)current, &buffer[PATTERN_MAX_SIZE], CHUNK_SIZE, &bytesRead);
            VirtualProtectEx(hProc, (void*)current, CHUNK_SIZE, oldProtect, &oldProtect);

            if (bytesRead == 0) {
                hProc = NULL;
            }
        }
    };

}
