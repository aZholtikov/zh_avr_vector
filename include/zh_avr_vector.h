#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        AVR_FAIL = -1,
        AVR_OK,
        AVR_ERR_NO_MEM,
        AVR_ERR_INVALID_ARG,
        AVR_ERR_INVALID_STATE,
        AVR_ERR_INVALID_SIZE,
        AVR_ERR_NOT_FOUND,
        AVR_ERR_NOT_SUPPORTED,
        AVR_ERR_TIMEOUT,
        AVR_ERR_INVALID_RESPONSE,
        AVR_ERR_INVALID_CRC,
        AVR_ERR_INVALID_VERSION,
        AVR_ERR_NOT_FINISHED,
        AVR_ERR_NOT_ALLOWED
    } avr_err_t;

    typedef struct // Main structure of vector data.
    {
        void **items;     // Array of pointers of vector items.
        uint8_t capacity; // Maximum capacity of the vector. @note Used to control the size of allocated memory for array of pointers of vector items. Usually equal to the current number of items in the vector. Automatically changes when items are added or deleted.
        uint8_t size;     // Number of items in the vector. @note Can be read with zh_avr_vector_get_size().
        uint8_t unit;     // Vector item size. @note Possible values from 1 to 255.
        bool status;      // Vector initialization status flag. @note Used to prevent execution of vector functions without prior vector initialization.
    } zh_avr_vector_t;

    /**
     * @brief Initialize vector.
     *
     * @param[in] vector Pointer to main structure of vector data.
     * @param[in] unit Size of vector item.
     *
     * @return AVR_OK if success or an error code otherwise.
     */
    avr_err_t zh_avr_vector_init(zh_avr_vector_t *vector, uint8_t unit);

    /**
     * @brief Deinitialize vector. Free all allocated memory.
     *
     * @param[in] vector Pointer to main structure of vector data.
     *
     * @return AVR_OK if success or an error code otherwise.
     */
    avr_err_t zh_avr_vector_free(zh_avr_vector_t *vector);

    /**
     * @brief Get current vector size.
     *
     * @param[in] vector Pointer to main structure of vector data.
     *
     * @return AVR_OK if success or an error code otherwise.
     */
    avr_err_t zh_avr_vector_get_size(zh_avr_vector_t *vector);

    /**
     * @brief Add item at end of vector.
     *
     * @param[in] vector Pointer to main structure of vector data.
     * @param[in] item Pointer to item for add.
     *
     * @return AVR_OK if success or an error code otherwise.
     */
    avr_err_t zh_avr_vector_push_back(zh_avr_vector_t *vector, void *item);

    /**
     * @brief Change item by index.
     *
     * @param[in] vector Pointer to main structure of vector data.
     * @param[in] index Index of item for change.
     * @param[in] item Pointer to new data of item.
     *
     * @return AVR_OK if success or an error code otherwise.
     */
    avr_err_t zh_avr_vector_change_item(zh_avr_vector_t *vector, uint8_t index, void *item);

    /**
     * @brief Get item by index.
     *
     * @param[in] vector Pointer to main structure of vector data.
     * @param[in] index Index of item for get.
     *
     * @return AVR_OK if success or an error code otherwise.
     */
    void *zh_avr_vector_get_item(zh_avr_vector_t *vector, uint8_t index);

    /**
     * @brief Delete item by index and shifts all elements in vector.
     *
     * @param[in] vector Pointer to main structure of vector data.
     * @param[in] index Index of item for delete.
     *
     * @return AVR_OK if success or an error code otherwise.
     */
    avr_err_t zh_avr_vector_delete_item(zh_avr_vector_t *vector, uint8_t index);

#ifdef __cplusplus
}
#endif