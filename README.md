# FreeRTOS based AVR library for vector (dynamic array)

## Features

1. Support of any data types.
2. The maximum size of the veсtor is 255 elements.

## Dependencies

1. [zh_avr_free_rtos](http://git.zh.com.ru/avr_libraries/zh_avr_free_rtos)

## Using

In an existing project, run the following command to install the component:

```text
cd ../your_project/lib
git clone http://git.zh.com.ru/avr_libraries/zh_avr_free_rtos
git clone http://git.zh.com.ru/avr_libraries/zh_avr_vector
```

In the application, add the component:

```c
#include "zh_avr_vector.h"
```

## Example

Create, add, read, modify and delete items:

```c
#include "avr/io.h"
#include "stdio.h"
#include "zh_avr_vector.h"

#define BAUD_RATE 9600
#define BAUD_PRESCALE (F_CPU / 16 / BAUD_RATE - 1)

int usart(char byte, FILE *stream)
{
    while ((UCSR0A & (1 << UDRE0)) == 0)
    {
    }
    UDR0 = byte;
    return 0;
}
FILE uart = FDEV_SETUP_STREAM(usart, NULL, _FDEV_SETUP_WRITE);

void vector_example_task(void *pvParameters)
{
    zh_avr_vector_t vector = {0};
    char example[10] = {0};
    printf("Free Heap %d.\n", xPortGetFreeHeapSize());
    zh_avr_vector_init(&vector, sizeof(example));
    printf("Initial vector size is: %d\n", zh_avr_vector_get_size(&vector));
    strcpy(example, "Item 1");
    zh_avr_vector_push_back(&vector, &example);
    strcpy(example, "Item 2");
    zh_avr_vector_push_back(&vector, &example);
    strcpy(example, "Item 3");
    zh_avr_vector_push_back(&vector, &example);
    strcpy(example, "Item 4");
    zh_avr_vector_push_back(&vector, &example);
    strcpy(example, "Item 5");
    zh_avr_vector_push_back(&vector, &example);
    printf("Add 5 items. New vector size is: %d\n", zh_avr_vector_get_size(&vector));
    for (uint16_t i = 0; i < zh_avr_vector_get_size(&vector); ++i)
    {
        printf("Item position %d is: %s\n", i, (char *)zh_avr_vector_get_item(&vector, i));
    }
    strcpy(example, "Item 6");
    zh_avr_vector_change_item(&vector, 3, &example);
    printf("Change item on 3 position.\n");
    for (uint16_t i = 0; i < zh_avr_vector_get_size(&vector); ++i)
    {
        printf("Item position %d is: %s\n", i, (char *)zh_avr_vector_get_item(&vector, i));
    }
    zh_avr_vector_delete_item(&vector, 2);
    printf("Delete item on 2 position. New vector size is: %d\n", zh_avr_vector_get_size(&vector));
    for (uint16_t i = 0; i < zh_avr_vector_get_size(&vector); ++i)
    {
        printf("Item position %d is: %s\n", i, (char *)zh_avr_vector_get_item(&vector, i));
    }
    zh_avr_vector_free(&vector);
    printf("Task Remaining Stack Size %d.\n", uxTaskGetStackHighWaterMark(NULL));
    printf("Free Heap %d.\n", xPortGetFreeHeapSize());
    vTaskDelete(NULL);
}

int main(void)
{
    UBRR0H = (BAUD_PRESCALE >> 8);
    UBRR0L = BAUD_PRESCALE;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    stdout = &uart;
    xTaskCreate(vector_example_task, "vector example task", 136, NULL, tskIDLE_PRIORITY, NULL);
    vTaskStartScheduler();
    return 0;
}
```
