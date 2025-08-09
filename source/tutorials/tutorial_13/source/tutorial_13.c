/*
 * FreeRTOS-Tutorials
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT-0
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/* Standard includes. */
#include <stdio.h>
#include <unistd.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define MAX_COUNT 100

/**
 * @brief Functions that implement FreeRTOS tasks.
 */
static void prvTask1( void * pvParams );
static void prvTask2( void * pvParams );

static void prvEvenTask( void * pvParams );
static void prvOddTask( void * pvParams );
/*-----------------------------------------------------------*/

/**
 * @brief Mutex used in this program.
 */
static SemaphoreHandle_t xMutex = NULL;

/*-----------------------------------------------------------*/
static volatile int count = 1;

/**
 * @brief Tutorial entry point.
 */
int main( void )
{
    BaseType_t xTaskCreationResult = pdFAIL;

    xMutex = xSemaphoreCreateMutex();
    configASSERT( xMutex != NULL );
    if (xMutex != NULL)
    {
        xTaskCreationResult = xTaskCreate( prvEvenTask,
                                        "Task1",
                                        configMINIMAL_STACK_SIZE,
                                        NULL,
                                        tskIDLE_PRIORITY,
                                        NULL );
        configASSERT( xTaskCreationResult == pdPASS );

        xTaskCreationResult = xTaskCreate( prvOddTask,
                                        "Task2",
                                        configMINIMAL_STACK_SIZE,
                                        NULL,
                                        tskIDLE_PRIORITY,
                                        NULL );
        configASSERT( xTaskCreationResult == pdPASS );
     }
#if 0
    /*
     * TODO 1 - Create a mutex using xSemaphoreCreateMutex API.
     *
     * Assign the return value to xMutex.
     */
    xMutex = xSemaphoreCreateMutex();
    configASSERT( xMutex != NULL );
#endif

    /* Start the scheduler. */
    vTaskStartScheduler();

    /* Should not reach here. */
    for( ;; )
    {

    }

    /* Just to make the compiler happy. */
    return 0;
}
/*-----------------------------------------------------------*/

static void prvTask1( void * pvParams )
{
    BaseType_t xSemaphoreGiveResult = pdFAIL;

    /* Silence warning about unused parameters. */
    ( void ) pvParams;

    for( ;; )
    {
        /*
         * TODO 2 - Take the mutex using xSemaphoreTake API.
         *
         * Use the following values for xSemaphoreTake parameters:
         * xSemaphore   xMutex
         * xBlockTime   portMAX_DELAY
         */
        xSemaphoreTake(xMutex, portMAX_DELAY);
        {
            fprintf( stderr, "Task 1 is running...\r\n" );
        }
        /*
         * TODO 3 - Give the mutex using xSemaphoreGive API.
         *
         * Use the following values for xSemaphoreGive parameters:
         * xSemaphore   xMutex
         *
         * Assign the return value to xSemaphoreGiveResult.
         */
        xSemaphoreGiveResult=  xSemaphoreGive(xMutex);
        configASSERT( xSemaphoreGiveResult == pdPASS );

        vTaskDelay( pdMS_TO_TICKS( 1000 ) );
    }

}
/*-----------------------------------------------------------*/

static void prvTask2( void * pvParams )
{
    BaseType_t xSemaphoreGiveResult = pdFAIL;

    /* Silence warning about unused parameters. */
    ( void ) pvParams;

    for( ;; )
    {
        /*
         * TODO 4 - Take the mutex using xSemaphoreTake API.
         *
         * Use the following values for xSemaphoreTake parameters:
         * xSemaphore   xMutex
         * xBlockTime   portMAX_DELAY
         */
        xSemaphoreTake(xMutex, portMAX_DELAY);
        {
            fprintf( stderr, "Task 2 is running...\r\n" );
        }
        /*
         * TODO 5 - Give the mutex using xSemaphoreGive API.
         *
         * Use the following values for xSemaphoreGive parameters:
         * xSemaphore   xMutex
         *
         * Assign the return value to xSemaphoreGiveResult.
         */
        xSemaphoreGiveResult = xSemaphoreGive(xMutex);
        configASSERT( xSemaphoreGiveResult == pdPASS );

        vTaskDelay( pdMS_TO_TICKS( 1000 ) );
    }
}

static void prvEvenTask( void * pvParams )
{
    (void)pvParams;

    for(;;)
    {
        // Lock the mutex
        xSemaphoreTake(xMutex, portMAX_DELAY);
        if (!(count & 1) && (count <= MAX_COUNT))
        {
            fprintf(stdout, "Even Task: %d\n", count++);
            fflush(stdout);
        }
        // Unlock mutex
        xSemaphoreGive(xMutex);
        vTaskDelay(pdMS_TO_TICKS(1000));
        taskYIELD(); // Let the other task run
    }

    vTaskDelete(NULL);
}

static void prvOddTask(void* pvParams)
{
    (void)pvParams;

    for(;;)
    {
        // Lock the mutex
        xSemaphoreTake(xMutex, portMAX_DELAY);
        if ((count & 1) && (count <= MAX_COUNT))
        {
            fprintf(stdout, "Odd Task:  %d\n", count++);
            fflush(stdout);
        }
        // Unlock mutex
        xSemaphoreGive(xMutex);
        vTaskDelay(pdMS_TO_TICKS(1000));
        taskYIELD(); // Let the other task run
    }

    vTaskDelete(NULL);
}
/*-----------------------------------------------------------*/
