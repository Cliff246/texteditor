#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fileioc.h>

#include "include/types.h"
#include "include/compare.h"

#define CHUNKSIZE 8

typedef struct ti_file ti_file_t;
struct ti_file
{
    ti_var_t file_ptr;
    char file_mode[3];
    char file_name[];
};

bool exists_file(char* name)
{
    if(name != NULL)
    {
        char* ifile_name;
        uint8_t *iterator = NULL;

        while((ifile_name = ti_Detect((void**)&iterator,name)) != NULL)
        {
            if(strcmp(ifile_name,name) == 0)
                return true;
        }
        return false;
    }
    os_ThrowError(-1);
    return false;
}

ti_file_t* new_file(char* iname)
{
    if(iname != NULL)
    {
        uint16_t sname = strlen(iname);
        char *fname = (char*)calloc(sname+2,1);
        if(fname != NULL)
        {
            bool available = false;
            memcpy(fname,iname,sname);
            for(uint8_t i = 0;i < 10;i++)
            {
                if(exists_file(fname))
                   fname[sname+1] = i + 0x30;
                else
                    available = true;
            }
            if(!available)
                return NULL;
        }
        else
            os_ThrowError(-1);
        ti_var_t var_file = ti_Open(fname,"w+");
        ti_file_t* wrap_file = (ti_file_t*)malloc(sizeof(ti_file_t) + strlen(fname+1));
        if(var_file != 0 && wrap_file != NULL)
        {
            wrap_file->file_ptr = var_file;
            strcpy(wrap_file->file_mode,"w+");
            strcpy(wrap_file->file_name,fname);
            free(fname);
            return wrap_file;
        }
        else
        {
            free(fname);
            return NULL;
        }
    }
    os_ThrowError(-1);
}

ti_file_t* open_file(char* name)
{
    if(name != NULL)
    {
        ti_var_t var_file = ti_Open(name,"r+");
        ti_file_t* wrap_file = (ti_file_t*)malloc(sizeof(ti_file_t) + strlen(name+1));
        if(!var_file && wrap_file != NULL)
        {
            wrap_file->file_ptr = var_file;
            strcpy(wrap_file->file_mode,"r+");
            strcpy(wrap_file->file_name,name);
            
            return wrap_file;
        }
    }
    os_ThrowError(-1);
    return NULL;
}

void close_file(ti_file_t* file)
{
    if(file != NULL)
    {
        ti_Close(file->file_ptr);
        free(file);
        file = NULL;
    }
    os_ThrowError(-1);
    return;
}

char* read_file(ti_file_t* file,uint16_t count)
{
    if(file != NULL)
    {
        char* mallowed[] = {"r","r+","w+","a+"};
        if(contain_string(file->file_mode,mallowed,3) == false)
            return NULL;
        if(count > ti_GetSize(file->file_ptr))
            os_ThrowError(-1);
        char* data = (char*)malloc(count);
        if(data != NULL)
        {
            ti_Read(data,CHUNKSIZE,count,file->file_ptr);
            return data;
        }
    }   
    os_ThrowError(-1);
    return NULL;
}

void write_file(ti_file_t* file,char* write,uint32_t size)
{
    if(write != NULL || file == NULL)
    {
        char* mallowed[] = {"w","r+","w+","a+"};
        if(contain_string(file->file_mode,mallowed,3) == false)
            return;
        if(size > ti_GetSize(file->file_ptr))
            os_ThrowError(-1);
        ti_Write(write,CHUNKSIZE,size,file->file_ptr);
        return;
    }
    os_ThrowError(-1);
    return;
}

void* get_ptr_file(ti_file_t* file)
{
    if(file != NULL && file->file_ptr != 0)
    {
        void* ptr = ti_GetDataPtr(file->file_ptr);
        return ptr;
    }
    os_ThrowError(-1);
    return NULL;
}

