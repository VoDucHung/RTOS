// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>
#include "task.h"
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;

void setup() 
{
  Serial.begin(9600); // Enable serial communication library.
  xTaskCreate(Task1, "Task1", 192, NULL, 2,&TaskHandle_1);
  xTaskCreate(Task2, "Task2", 192, NULL, 3,&TaskHandle_2);

}

void loop() 
{
// Do nothing as schduler will allocated CPU to Task1 and Task2 automatically
}
/* Task1 with priority 1 */
void Task1(void* pvParameters)
{
  while(1)
  {
    Serial.println("Task1 Running"); // print "Task1 Running" on Arduino Serial Monitor
    Serial.print("Priority of Task1 = ");
    Serial.println(uxTaskPriorityGet(TaskHandle_1));
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  delay(500);
  }
}

/* Task2 with priority 2 */
void Task2(void* pvParameters)
{ 
  while(1)
  {
  Serial.println("Task2 Running"); // print "Task1 Running" on Arduino Serial Monitor
  Serial.print("Priority of Task2 = ");
  Serial.println(uxTaskPriorityGet(TaskHandle_2));
  vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
}