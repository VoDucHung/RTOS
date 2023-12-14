// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>
#include "task.h"


void setup() 
{
  Serial.begin(9600); // Enable serial communication library.
  pinMode(4, OUTPUT);  // define LED1 pin as a digital output 

  xTaskCreate(Task1, "LED_ON", 100, NULL, 2, NULL);
  xTaskCreate(Task2, "LED_OFF", 100, NULL, 2, NULL);
  vTaskStartScheduler();
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
    digitalWrite(4, HIGH); // sets the digital pin 4 on
  // vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  delay(500);
  }
}

/* Task2 with priority 2 */
void Task2(void* pvParameters)
{ 
  while(1)
  {
  digitalWrite(4, LOW); // sets the digital pin 4 low
  Serial.println("Task2 Running"); // print "Task1 Running" on Arduino Serial Monitor
  delay(500);
  }
}