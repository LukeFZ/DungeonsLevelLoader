#pragma once
#include <diet-ue.h>

namespace spy {

    extern FUObjectArray *GUObjectArray;
    extern TNameEntryArray *GNames;
    extern UEngine* GEngine;
    
	// If we move this to spy:: where we have GUObjectArray we could make it work
	// exactly like the original FRawObjectIterator
	class FRawObjectIterator : public FUObjectArray::TIterator
	{
	public:
		FRawObjectIterator( bool bOnlyGCedObjects = false ) : 
            FUObjectArray::TIterator(*GUObjectArray, bOnlyGCedObjects)
		{
		}

		/**
		 * Iterator dereference
		 * @return	the object pointer pointed at by the iterator
		 */
		FORCEINLINE ::FUObjectItem* operator*() const
		{
			// casting UObjectBase to UObject for clients
			return GetObject();
		}
		/**
		 * Iterator dereference
		 * @return	the object pointer pointed at by the iterator
		 */
		FORCEINLINE ::FUObjectItem* operator->() const
		{
			return GetObject();
		}
	};


}