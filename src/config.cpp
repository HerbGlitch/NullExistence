#include "config.hpp"
#include <string.h>
#include <stdlib.h>
#include "types.hpp"

namespace tbyte {
    void initConfig(){
        arc::config->addKey("TextMap", TextMap_Read, TextMap_Delete);
        arc::config->addKey("Tile"   , Tile_Read   , Tile_Delete   );
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

    uint8_t TextMap_Read(ARC_Config *config, ARC_String *string, void **value){
        ARC_Config_Get(config, string, value);
        if(*value){
            return 1;
        }

        ARC_String *path;
        ARC_ConfigKey_Read_String(config, string, (void **)&path);
        if(arc_errno){
            arc_errno = 0;
            return 0;
        }

        ARC_String *textMapString;
        ARC_IO_FileToStr(path, &textMapString);
        if(arc_errno){
            ARC_ConfigKey_Delete_String(config, string, (void *)path);
            arc_errno = 0;
            return 0;
        }

        TextMap *textMap = new TextMap;
        textMap->size = { 0, 1 };

        //TODO: add error checks
        bool first = true;
        for(uint64_t i = 0; i < textMapString->length; i++){
            if(textMapString->data[i] == '\n'){
                first = false;
                textMap->size.y++;
            }

            if(!first){
                continue;
            }

            //get first row tiles width
            textMap->size.x++;
        }

        textMap->tiles = new char *[textMap->size.y];
        for(int32_t y = 0; y < textMap->size.y; y++){
            textMap->tiles[y] = new char[textMap->size.x];
        }

        for(int32_t y = 0; y < textMap->size.y; y++){
            for(int32_t x = 0; x < textMap->size.x; x++){
                textMap->tiles[y][x] = textMapString->data[(y * (textMap->size.x + 1)) + x];
            }
        }

        *value = (void *)textMap;

        ARC_String_Destroy(textMapString);

        ARC_ConfigKey_Delete_String(config, string, (void *)path);

        return 0;
    }

    uint8_t Tile_Read(ARC_Config *config, ARC_String *string, void **value){
        ARC_Config_Get(config, string, value);
        if(*value){
            return 1;
        }

        if(string->data[0] != '{' || string->data[string->length - 1] != '}'){
            ARC_DEBUG_LOG(arc_errno, "in Tile_Read(config, string, value); no matching curly braces: %s", string->data);
            arc_errno = ARC_ERRNO_DATA;
            return 0;
        }

        //remove curly braces from string
        ARC_String *subString;
        uint64_t index = 1;
        ARC_String_CopySubstring(&subString, string, index, string->length - (index + 1));

        //id
        uint32_t *id;
        StripAndReadNextType(config, &subString, ",", ARC_ConfigKey_Read_Uint32_t, (void **)&id);
        if(arc_errno){
            free(id);
            return 0;
        }

        //pos
        ARC_Point *pos;
        StripAndReadNextType(config, &subString, "},", ARC_Point_Read, (void **)&pos);
        if(arc_errno){
            free(id);
            free(pos);
            return 0;
        }

        //types
        uint32_t *types;
        StripAndReadNextType(config, &subString, NULL, ARC_ConfigKey_Read_Uint32_t, (void **)&types);
        if(arc_errno){
            free(id);
            free(pos);
            free(types);
            return 0;
        }

        *value = (void *) new Tile { *id, *pos, *types };

        //cleanup
        free(id);
        free(pos);
        free(types);

        ARC_String_Destroy(subString);

        return 0;
    }

    void TextMap_Delete(ARC_Config *config, ARC_String *string, void *value){
        for(int32_t y = 0; y < ((TextMap *)value)->size.y; y++){
            delete [] ((TextMap *)value)->tiles[y];
        }

        delete [] ((TextMap *)value)->tiles;
        delete (TextMap *)value;
    }

    void Tile_Delete(ARC_Config *config, ARC_String *string, void *value){
        delete (Tile *)value;
    }
}