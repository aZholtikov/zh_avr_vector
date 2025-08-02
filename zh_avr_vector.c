#include "zh_avr_vector.h"

static avr_err_t _resize(zh_avr_vector_t *vector, uint16_t capacity);

avr_err_t zh_avr_vector_init(zh_avr_vector_t *vector, uint16_t unit)
{
    if (vector == NULL || unit == 0)
    {
        return AVR_ERR_INVALID_ARG;
    }
    if (vector->status == true)
    {
        return AVR_ERR_INVALID_STATE;
    }
    vector->capacity = 0;
    vector->size = 0;
    vector->unit = unit;
    vector->status = true;
    return AVR_OK;
}

avr_err_t zh_avr_vector_free(zh_avr_vector_t *vector)
{
    if (vector == NULL)
    {
        return AVR_ERR_INVALID_ARG;
    }
    if (vector->status == false)
    {
        return AVR_ERR_INVALID_STATE;
    }
    for (uint16_t i = 0; i < vector->size; ++i)
    {
        free(vector->items[i]);
    }
    vector->status = false;
    return AVR_OK;
}

avr_err_t zh_avr_vector_get_size(zh_avr_vector_t *vector)
{
    if (vector == NULL || vector->status == false)
    {
        return AVR_FAIL;
    }
    return vector->size;
}

avr_err_t zh_avr_vector_push_back(zh_avr_vector_t *vector, void *item)
{
    if (vector == NULL || item == NULL)
    {
        return AVR_ERR_INVALID_ARG;
    }
    if (vector->status == false)
    {
        return AVR_ERR_INVALID_STATE;
    }
    if (vector->capacity == vector->size)
    {
        if (_resize(vector, vector->capacity + 1) == AVR_ERR_NO_MEM)
        {
            return AVR_ERR_NO_MEM;
        }
    }
    vector->items[vector->size] = calloc(1, vector->unit);
    if (vector->items[vector->size] == NULL)
    {
        return AVR_ERR_NO_MEM;
    }
    memcpy(vector->items[vector->size++], item, vector->unit);
    return AVR_OK;
}

avr_err_t zh_avr_vector_change_item(zh_avr_vector_t *vector, uint16_t index, void *item)
{
    if (vector == NULL || item == NULL)
    {
        return AVR_ERR_INVALID_ARG;
    }
    if (vector->status == false)
    {
        return AVR_ERR_INVALID_STATE;
    }
    if (index < vector->size)
    {
        memcpy(vector->items[index], item, vector->unit);
        return AVR_OK;
    }
    return AVR_FAIL;
}

void *zh_avr_vector_get_item(zh_avr_vector_t *vector, uint16_t index)
{
    if (vector == NULL)
    {
        return NULL;
    }
    if (vector->status == false)
    {
        return NULL;
    }
    if (index < vector->size)
    {
        void *item = vector->items[index];
        return item;
    }
    else
    {
        return NULL;
    }
}

avr_err_t zh_avr_vector_delete_item(zh_avr_vector_t *vector, uint16_t index)
{
    if (vector == NULL)
    {
        return AVR_ERR_INVALID_ARG;
    }
    if (vector->status == false)
    {
        return AVR_ERR_INVALID_STATE;
    }
    if (index < vector->size)
    {
        free(vector->items[index]);
        for (uint8_t i = index; i < (vector->size - 1); ++i)
        {
            vector->items[i] = vector->items[i + 1];
            vector->items[i + 1] = NULL;
        }
        --vector->size;
        _resize(vector, vector->capacity - 1);
        return AVR_OK;
    }
    return AVR_FAIL;
}

static avr_err_t _resize(zh_avr_vector_t *vector, uint16_t capacity)
{
    if (capacity != 0)
    {
        vector->items = realloc(vector->items, sizeof(void *) * capacity);
        if (vector->items == NULL)
        {
            return AVR_ERR_NO_MEM;
        }
    }
    vector->capacity = capacity;
    return AVR_OK;
}