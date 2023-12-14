#include <Arduino_FreeRTOS.h>
#include "task.h"

TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;
TaskHandle_t TaskHandle_4;

void setup()
{  
  Serial.begin(9600);
  Serial.println(F("In Setup function"));

  /* Create 4-tasks with priorities 1-4. Capture the Task details to respective handlers */
  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, &TaskHandle_1);
  xTaskCreate(MyTask2, "Task2", 100, NULL, 2, &TaskHandle_2);  
  xTaskCreate(MyTask3, "Task3", 100, NULL, 3, &TaskHandle_3);
  xTaskCreate(MyTask4, "Task4", 100, NULL, 4, &TaskHandle_4); 
}

void loop()
{ // Hooked to Idle Task, will run when CPU is Idle
  Serial.println(F("Loop function"));
  delay(50);
}

/* Task1 with priority 1 */
static void MyTask1(void* pvParameters)
{ 
    Serial.println(F("Task1 Resuming Task2"));
    vTaskResume(TaskHandle_2);
    
    Serial.println(F("Task1 Resuming Task3"));
    vTaskResume(TaskHandle_3);

    Serial.println(F("Task1 Resuming Task4"));
    vTaskResume(TaskHandle_4);

    Serial.println(F("Task1 Deleting Itself"));
    vTaskDelete(TaskHandle_1);
}

/* Task2 with priority 2 */
static void MyTask2(void* pvParameters)
{        
    Serial.println(F("Task2, Deleting itself"));
    vTaskDelete(NULL);     //Delete own task by passing NULL(TaskHandle_2 can also be used)
    delay(1000);
}

/* Task3 with priority 3 */
static void MyTask3(void* pvParameters)
{
    Serial.println(F("Task3, Deleting Itself"));
    vTaskDelete(NULL);     //Delete own task by passing NULL(TaskHandle_3 can also be used)  
    delay(1000);

}

/* Task4 with priority 4 */
static void MyTask4(void* pvParameters)
{
    Serial.println(F("Task4 Running, Suspending all tasks"));
    vTaskSuspend(TaskHandle_2); //Suspend Task2/3
    vTaskSuspend(TaskHandle_3);
    vTaskSuspend(NULL); //Suspend Own Task

    Serial.println(F("Back in Task4, Deleting Itself"));
    vTaskDelete(TaskHandle_4);      
    delay(1000);

}