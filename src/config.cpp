#include "config.hpp"
#include <string.h>
#include <stdlib.h>

namespace tbyte {
    void initConfig(){
    }

    uint64_t GetIndexAndErrorCheck(ARC_String *string, char *search, uint64_t searchLength){
        uint64_t separator = ARC_String_FindCString(string, search, searchLength);

        if(separator == ~(uint64_t)0){
            arc_errno = ARC_ERRNO_DATA;
        }

        return separator;
    }

    void StripAndReadNextType(ARC_Config *config, ARC_String **subString, const char *search, ARC_ConfigKeyRead ReadFn, void **value){
        uint64_t separator = (*subString)->length;
        uint64_t typeLen = separator - 1;
        if(search != NULL){
            separator = GetIndexAndErrorCheck(*subString, (char *)search, strlen(search));
            typeLen = (separator - 1) + (strlen(search) - 1);
            if(arc_errno){
                ARC_DEBUG_LOG(arc_errno, "in StripAndReadNextType(config, subString, search, ReadFn, value), no matching search: %s", search);
                return;
            }
        }

        //get and store type
        ARC_String *typeString;
        ARC_String_CopySubstring(&typeString, *subString, 0, typeLen);
        ARC_String *oldSubString = typeString;
        ARC_String_StripEndsWhitespace(oldSubString, &typeString);
        ARC_String_Destroy(oldSubString);
        ReadFn(config, typeString, value);
        ARC_String_Destroy(typeString);

        //if next is null don't update substring
        if(search == NULL){
            return;
        }

        //update substring
        uint64_t index = separator + 1;
        oldSubString = *subString;
        ARC_String_CopySubstring(subString, oldSubString, index, (*subString)->length - index);
        ARC_String_Destroy(oldSubString);
    }
}