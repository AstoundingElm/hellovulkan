#pragma once
#define TRUE 0
#define FALSE 1
#include <stdlib.h>
#include <string.h>
//#include <vulkan/vulkan.h>
/*
Memory layout
long unsigned int capacity = number elements that can be held
long unsigned int length = number of elements currently contained
long unsigned int stride = size of each element in bytes
void* elements
*/

typedef enum memory_tag {
    // For temporary use. Should be assigned one of the below or have a new tag created.
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_DICT,
    MEMORY_TAG_RING_QUEUE,
    MEMORY_TAG_BST,
    MEMORY_TAG_STRING,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_JOB,
    MEMORY_TAG_TEXTURE,
    MEMORY_TAG_MATERIAL_INSTANCE,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_GAME,
    MEMORY_TAG_TRANSFORM,
    MEMORY_TAG_ENTITY,
    MEMORY_TAG_ENTITY_NODE,
    MEMORY_TAG_SCENE,

    MEMORY_TAG_MAX_TAGS
} memory_tag;

static const char* memory_tag_strings[MEMORY_TAG_MAX_TAGS] = {
    "UNKNOWN    ",
    "ARRAY      ",
    "DARRAY     ",
    "DICT       ",
    "RING_QUEUE ",
    "BST        ",
    "STRING     ",
    "APPLICATION",
    "JOB        ",
    "TEXTURE    ",
    "MAT_INST   ",
    "RENDERER   ",
    "GAME       ",
    "TRANSFORM  ",
    "ENTITY     ",
    "ENTITY_NODE",
    "SCENE      "};

enum {
    DARRAY_CAPACITY,
    DARRAY_LENGTH,
    DARRAY_STRIDE,
    DARRAY_FIELD_LENGTH
};

void* kallocate(long unsigned int size, memory_tag tag);

void kfree(void* block, long unsigned int size, memory_tag tag);

void* kzero_memory(void* block, long unsigned int size);
void* kcopy_memory(void* dest, const void* source, long unsigned int size);

void* kset_memory(void* dest, int value, long unsigned int size);


void* _darray_create(long unsigned int length, __UINT64_TYPE__ stride);
void _darray_destroy(void* array);

long unsigned int _darray_field_get(void* array, long unsigned int field);
void _darray_field_set(void* array, long unsigned int field, long unsigned int value);

void* _darray_resize(void* array);

void* _darray_push(void* array, const void* value_ptr);
void _darray_pop(void* array, void* dest);

void* _darray_pop_at(void* array, long unsigned int index, void* dest);
void* _darray_insert_at(void* array, long unsigned int index, void* value_ptr);

#define DARRAY_DEFAULT_CAPACITY 1
#define DARRAY_RESIZE_FACTOR 2

#define darray_create(type) \
    _darray_create(DARRAY_DEFAULT_CAPACITY, sizeof(type))

#define darray_reserve(type, capacity) \
    _darray_create(capacity, sizeof(type))

#define darray_destroy(array) _darray_destroy(array);

#define darray_push(array, value)           \
    {                                       \
        typeof(value) temp = value;         \
        array = _darray_push(array, &temp); \
    }
// NOTE: could use __auto_type for temp above, but intellisense
// for VSCode flags it as an unknown type. typeof() seems to
// work just fine, though. Both are GNU extensions.

#define darray_pop(array, value_ptr) \
    _darray_pop(array, value_ptr)

#define darray_insert_at(array, index, value)           \
    {                                                   \
        typeof(value) temp = value;                     \
        array = _darray_insert_at(array, index, &temp); \
    }

#define darray_pop_at(array, index, value_ptr) \
    _darray_pop_at(array, index, value_ptr)

#define darray_clear(array) \
    _darray_field_set(array, DARRAY_LENGTH, 0)

#define darray_capacity(array) \
    _darray_field_get(array, DARRAY_CAPACITY)

#define darray_length(array) \
    _darray_field_get(array, DARRAY_LENGTH)

#define darray_stride(array) \
    _darray_field_get(array, DARRAY_STRIDE)

#define darray_length_set(array, value) \
    _darray_field_set(array, DARRAY_LENGTH, value)
    