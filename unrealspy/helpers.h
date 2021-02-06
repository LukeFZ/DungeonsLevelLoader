#pragma once

#include "unrealspy.h"

namespace spy {
	SPYAPI bool GetName(UObject *obj, ANSICHAR (&OutName)[1024]);
	SPYAPI ANSICHAR *GetName(UObject *obj);
	SPYAPI UObject *FindObjectByName(char *ObjectName, char *ClassName);

}
