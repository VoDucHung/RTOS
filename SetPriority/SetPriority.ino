// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>
#include "task.h"
TaskHandle_t TaskHandle_1;

void setup() 
{
  Serial.begin(9600); // Enable serial communication library.
  xTaskCreate(Task1, "Task1", 192, NULL, 2,&TaskHandle_1);

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
    delay(1000);
    Serial.println("Set priority of Task1 from 2 to 3");
    vTaskPrioritySet(TaskHandle_1, 3);
    Serial.print("Priority of Task1 = ");
    Serial.println(uxTaskPriorityGet(TaskHandle_1));
    delay(1000);
    Serial.println("Set priority of Task1 from 3 to 2");
    vTaskPrioritySet(TaskHandle_1, 2);
    Serial.print("Priority of Task1 = ");
    Serial.println(uxTaskPriorityGet(TaskHandle_1));
    delay(1000);
    vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for one second
  }
}
