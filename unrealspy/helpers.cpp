#include "helpers.h"
#include "unrealspy.h"
#include "unreal_impl.h"

namespace spy {

	bool GetName(UObject *obj, ANSICHAR (&OutName)[1024]) {
		FName name = obj->GetFName();
		TNameEntryArray& Names = *GNames;

		// Code from the ue source.
		const NAME_INDEX Index = name.GetDisplayIndex();
		const FNameEntry* const NameEntry = Names[Index];

		// GetComparisonIndex() seems to be the same thing as display index?

		NameEntry->GetAnsiName(OutName);
        return true;
	}

	ANSICHAR *GetName(UObject *obj) {
        if (obj == nullptr) {
            return nullptr;
        }

        // Give us direct access to the name char*
        class QuickName : public ::FNameEntry {
        public:
            FORCEINLINE const void *GetPtr()
            {
                //if (IsWide()) {
                //	return &WideName[0];
                //} else {
                    return &AnsiName[0];
                //} 
            }
        };

		FName name = obj->GetFName();
		TNameEntryArray& Names = *GNames;

		// Code from the ue source.
		const NAME_INDEX Index = name.GetDisplayIndex();

        auto NameEntry = (QuickName*)Names[Index];
        return (ANSICHAR*)NameEntry->GetPtr();
	}

	UObject *FindObjectByName(char *ObjectName, char *ClassName) {

        for (FRawObjectIterator It(false); It; ++It) {

            FUObjectItem *item = *It;
            UObject *obj = (UObject*)item->Object;

            // if objectName is requested but doesn't match, continue
            if (ObjectName != nullptr && strcmp(GetName(obj), ObjectName) != 0) {
                continue;
            }

            // if className is requested but doesn't match, continue
            if (ClassName != nullptr && strcmp(GetName(obj->GetClass()), ClassName) != 0) {
                continue;
            }

            return obj;
        }
        return nullptr;
	}
}