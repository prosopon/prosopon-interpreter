#include "pro_alloc.h"

#include <stdlib.h>


// Based on lua_alloc 
void* simple_alloc(void* ptr, size_t nsize)
{
    if (0 == nsize)
    {
        free(ptr);
        return 0;
    }
    else
        return realloc(ptr, nsize);
}
