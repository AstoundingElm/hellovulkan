#include "darray.h"
#include <stdio.h>
#include <stdlib.h>

void* kallocate(long unsigned int size, memory_tag tag) {
    
    

    // TODO: Memory alignment
    void* block = malloc(size);
    (void *)memset(block, 0, size);
    return block;
}

void kfree(void* block, long unsigned int size, memory_tag tag) {
    
    
    // TODO: Memory alignment
    free(block);
}

void* kzero_memory(void* block, long unsigned int size) {
    return memset(block, 0, size);
}

void* kcopy_memory(void* dest, const void* source, long unsigned int size) {
    return memcpy(dest, source, size);
}

void* kset_memory(void* dest, int value, long unsigned int size) {
    return memset(dest, value, size);
}

void* _darray_create(long unsigned int length, long unsigned int stride) {
    long unsigned int header_size = DARRAY_FIELD_LENGTH * sizeof(long unsigned int);
    long unsigned int array_size = length * stride;
    long unsigned int* new_array = (unsigned long *)kallocate(header_size + array_size, MEMORY_TAG_DARRAY);
    kset_memory(new_array, 0, header_size + array_size);
    new_array[DARRAY_CAPACITY] = length;
   
    new_array[DARRAY_LENGTH] = 0;
  
    new_array[DARRAY_STRIDE] = stride;
    
    return (void*)(new_array + DARRAY_FIELD_LENGTH);
    //typeof(int);
}

void _darray_destroy(void* array) {
    long unsigned int* header = (long unsigned int*)array - DARRAY_FIELD_LENGTH;
    long unsigned int header_size = DARRAY_FIELD_LENGTH * sizeof(long unsigned int);
    long unsigned int total_size = header_size + header[DARRAY_CAPACITY] * header[DARRAY_STRIDE];
    kfree(header, total_size, MEMORY_TAG_DARRAY);
}

long unsigned int _darray_field_get(void* array, long unsigned int field) {
    long unsigned int* header = (long unsigned int*)array - DARRAY_FIELD_LENGTH;
    return header[field];
}

void _darray_field_set(void* array, long unsigned int field, long unsigned int value) {
    long unsigned int* header = (long unsigned int*)array - DARRAY_FIELD_LENGTH;
    header[field] = value;
}

void* _darray_resize(void* array) {
    long unsigned int length = darray_length(array);
    long unsigned int stride = darray_stride(array);
    void* temp = _darray_create(
        (DARRAY_RESIZE_FACTOR * darray_capacity(array)),
        stride);
    kcopy_memory(temp, array, length * stride);

    _darray_field_set(temp, DARRAY_LENGTH, length);
    _darray_destroy(array);
    return temp;
}

void* _darray_push(void* array, const void* value_ptr) {
    long unsigned int length = darray_length(array);
    long unsigned int stride = darray_stride(array);
    if (length >= darray_capacity(array)) {
        array = _darray_resize(array);
    }

    long unsigned int addr = (long unsigned int)array;
    addr += (length * stride);
    kcopy_memory((void*)addr, value_ptr, stride);
    _darray_field_set(array, DARRAY_LENGTH, length + 1);
    return array;
}

void _darray_pop(void* array, void* dest) {
    long unsigned int length = darray_length(array);
    long unsigned int stride = darray_stride(array);
    long unsigned int addr = (long unsigned int)array;
    addr += ((length - 1) * stride);
    kcopy_memory(dest, (void*)addr, stride);
    _darray_field_set(array, DARRAY_LENGTH, length - 1);
}

void* _darray_pop_at(void* array, long unsigned int index, void* dest) {
    long unsigned int length = darray_length(array);
    long unsigned int stride = darray_stride(array);
    if (index >= length) {
       
        return array;
    }

    long unsigned int addr = (long unsigned int)array;
    kcopy_memory(dest, (void*)(addr + (index * stride)), stride);

    // If not on the last element, snip out the entry and copy the rest inward.
    if (index != length - 1) {
        kcopy_memory(
            (void*)(addr + (index * stride)),
            (void*)(addr + ((index + 1) * stride)),
            stride * (length - index));
    }

    _darray_field_set(array, DARRAY_LENGTH, length - 1);
    return array;
}

void* _darray_insert_at(void* array, long unsigned int index, void* value_ptr) {
    long unsigned int length = darray_length(array);
    long unsigned int stride = darray_stride(array);
    if (index >= length) {
        
        return array;
    }
    if (length >= darray_capacity(array)) {
        array = _darray_resize(array);
    }

    long unsigned int addr = (long unsigned int)array;

    // If not on the last element, copy the rest outward.
    if (index != length - 1) {
        kcopy_memory(
            (void*)(addr + ((index + 1) * stride)),
            (void*)(addr + (index * stride)),
            stride * (length - index));
    }

    // Set the value at the index
    kcopy_memory((void*)(addr + (index * stride)), value_ptr, stride);

    _darray_field_set(array, DARRAY_LENGTH, length + 1);
    return array;

    
}