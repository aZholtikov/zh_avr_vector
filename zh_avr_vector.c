#include "zh_avr_vector.h"

#define ZH_ERROR_CHECK(cond, err, ...) \
    if (!(cond))                       \
    {                                  \
        return err;                    \
    }

static avr_err_t _resize(zh_avr_vector_t *vector, uint8_t capacity);

avr_err_t zh_avr_vector_init(zh_avr_vector_t *vector, uint8_t unit)
{
    ZH_ERROR_CHECK((vector != NULL || unit != 0), AVR_ERR_INVALID_ARG);
    ZH_ERROR_CHECK(vector->status != true, AVR_ERR_INVALID_STATE);
    vector->capacity = 0;
    vector->size = 0;
    vector->unit = unit;
    vector->status = true;
    return AVR_OK;
}

avr_err_t zh_avr_vector_free(zh_avr_vector_t *vector)
{
    ZH_ERROR_CHECK(vector != NULL, AVR_ERR_INVALID_ARG);
    ZH_ERROR_CHECK(vector->status != false, AVR_ERR_INVALID_STATE);
    for (uint8_t i = 0; i < vector->size; ++i)
    {
        vPortFree(vector->items[i]);
    }
    vector->status = false;
    vPortFree(vector->items);
    return AVR_OK;
}

avr_err_t zh_avr_vector_get_size(zh_avr_vector_t *vector)
{
    ZH_ERROR_CHECK((vector != NULL || vector->status != false), AVR_FAIL);
    return vector->size;
}

avr_err_t zh_avr_vector_push_back(zh_avr_vector_t *vector, void *item)
{
    ZH_ERROR_CHECK((vector != NULL || item != NULL), AVR_ERR_INVALID_ARG);
    ZH_ERROR_CHECK(vector->status != false, AVR_ERR_INVALID_STATE);
    if (vector->capacity == vector->size)
    {
        ZH_ERROR_CHECK((_resize(vector, vector->capacity + 1) != AVR_ERR_NO_MEM), AVR_ERR_NO_MEM);
    }
    vector->items[vector->size] = pvPortCalloc(1, vector->unit);
    ZH_ERROR_CHECK(vector->items[vector->size] != NULL, AVR_ERR_NO_MEM);
    memcpy(vector->items[vector->size++], item, vector->unit);
    return AVR_OK;
}

avr_err_t zh_avr_vector_change_item(zh_avr_vector_t *vector, uint8_t index, void *item)
{
    ZH_ERROR_CHECK((vector != NULL || item != NULL), AVR_ERR_INVALID_ARG);
    ZH_ERROR_CHECK(vector->status != false, AVR_ERR_INVALID_STATE);
    if (index < vector->size)
    {
        memcpy(vector->items[index], item, vector->unit);
        return AVR_OK;
    }
    return AVR_FAIL;
}

void *zh_avr_vector_get_item(zh_avr_vector_t *vector, uint8_t index)
{
    ZH_ERROR_CHECK((vector != NULL || vector->status != false), NULL);
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

avr_err_t zh_avr_vector_delete_item(zh_avr_vector_t *vector, uint8_t index)
{
    ZH_ERROR_CHECK(vector != NULL, AVR_ERR_INVALID_ARG);
    ZH_ERROR_CHECK(vector->status != false, AVR_ERR_INVALID_STATE);
    if (index < vector->size)
    {
        vPortFree(vector->items[index]);
        for (uint8_t i = index; i < (vector->size - 1); ++i)
        {
            vector->items[i] = vector->items[i + 1];
            vector->items[i + 1] = NULL;
        }
        --vector->size;
        ZH_ERROR_CHECK((_resize(vector, vector->capacity - 1) != AVR_ERR_NO_MEM), AVR_ERR_NO_MEM);
        return AVR_OK;
    }
    return AVR_FAIL;
}

static avr_err_t _resize(zh_avr_vector_t *vector, uint8_t capacity)
{
    if (capacity != 0)
    {
        void *items = pvPortCalloc(capacity, sizeof(void *) * capacity);
        ZH_ERROR_CHECK(items != NULL, AVR_ERR_NO_MEM);
        memcpy(items, vector->items, sizeof(void *) * capacity);
        vPortFree(vector->items);
        vector->items = items;
    }
    vector->capacity = capacity;
    return AVR_OK;
}