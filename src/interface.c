#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <tice.h>
#include <fontlibc.h>

#include "include/types.h"
#include "include/fileio.h"

#define NUM_OF_SETTINGS 0

#define SETTINGS_FILE "textsetting"

typedef struct text_file text_file_t;

typedef struct contents_block contents_block_t;

/*
block size = the total allocated memory in a block
block length = length of the block string which should just be strlen but block data will not be terminated by a 0
block font id = the index of font_used in the text file
block data = the ptr to where the strings are being stored
*/
struct contents_block
{
    contents_block_t* block_next; 
    uint16_t block_size,block_length; 
    uint8_t block_font_id;
    char *block_data;
};

struct text_file
{
    ti_file_t* file_archive;
    uint16_t curor_pos;
    contents_block_t* block_head;
    uint8_t file_settings[NUM_OF_SETTINGS], fonts_used_size;  
    fontlib_font_t* fonts_used[];
 
};

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

ti_file_t* new_default_settings()
{
    ti_file_t *f_settings = new_file(SETTINGS_FILE);
    if(f_settings != NULL)
    {
        //settings shit
    }
    else
        os_ThrowError(-1);
    return f_settings;
}

uint8_t* get_default_settings()
{
    ti_file_t* f_settings;
    if(!exists_file(SETTINGS_FILE))
        f_settings = new_default_settings();
    if((f_settings = open_file(SETTINGS_FILE))!= NULL)
    {
        uint8_t* settings = (uint8_t*)malloc(NUM_OF_SETTINGS);
        if(settings != NULL)
        {
            //settings shit
            return settings;
        }
        
    }
    os_ThrowError(-1);
    return NULL;
}

text_file_t* new_text_file(char* name,fontlib_font_t* fonts[],uint8_t fonts_used)
{  
    text_file_t* t_file = (text_file_t*)calloc(1,sizeof(text_file_t) + (sizeof(fontlib_font_t) * fonts_used));
    name = (name!=NULL)? name : "untilted";
    ti_file_t* t_archive = (exists_file(name))? open_file(name):new_file(name);
    if(t_file != NULL && t_file != NULL)
    {
        uint8_t *settings = get_default_settings();
        memcpy(t_file->fonts_used,fonts,fonts_used);
        memcpy(t_file->file_settings,settings,NUM_OF_SETTINGS);
        t_file->fonts_used_size = fonts_used;
        t_file->curor_pos = 0;
        t_file->file_archive = t_archive;
        t_file->block_head = NULL;
        free(settings);
        return t_file;
    }  
    os_ThrowError(-1);    
    return NULL;
}