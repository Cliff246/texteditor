#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <tice.h>
#include <fontlibc.h>

#include "include/types.h"
#include "include/fileio.h"

fontlib_font_pack_t *get_font_pack(char* name)
{
    if(name == NULL)
        os_ThrowError(-1);
    ti_file_t* file = open_file(name);
    fontlib_font_pack_t* pack = get_ptr_file(file);
    close_file(file);
    return pack;
}

fontlib_font_t* font_from_pack(fontlib_font_pack_t* pack,uint8_t pos)
{
    if(pack != NULL)
    {
        fontlib_font_t *font = fontlib_GetFontByIndexRaw(pack,pos);
        if(font != NULL)
            return font;
        //idk why
    }
    os_ThrowError(-1);
    return NULL;
}

