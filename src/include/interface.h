#pragma once

#include <stdint.h>
#include <fontlibc.h>

#include "include/types.h"
#include "fileio.h"

typedef struct text_file text_file_t;

typedef struct contents_block contents_block_t;

struct contents_block;

struct text_file;

fontlib_font_pack_t *get_font_pack(char* name);

fontlib_font_t* font_from_pack(fontlib_font_pack_t* pack,uint8_t pos);

ti_file_t* new_default_settings();

uint8_t* get_default_settings();

text_file_t* new_text_file(char* name,fontlib_font_t* fonts[],uint8_t fonts_used);