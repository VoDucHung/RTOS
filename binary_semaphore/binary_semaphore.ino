
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

TaskHandle_t task1_t;
TaskHandle_t task2_t;

SemaphoreHandle_t Sem_Handle;

void setup()
{
  Serial.begin(9600);
  Sem_Handle = xSemaphoreCreateBinary(); //Create Binary Semaphore
  xTaskCreate(&task1,"task1",192,NULL,1,&task1_t);
  xTaskCreate(&task2,"task2",192,NULL,1,&task2_t);
  xTaskCreate(&task3,"task3",192, NULL,1,NULL);

}

void task1(void *pvPara){
  while(1){
  Serial.println("Task1 Running");
  vTaskDelay(10);
  }
}

void task2(void *pvPara){
  while(1){
  xSemaphoreTake(Sem_Handle,portMAX_DELAY);
  Serial.println("Task2 Running");
  vTaskDelay(10);

}
}

void task3(void *pvPara){
  while(1){
  while (Serial.available() > 0) {
        // display each character to the LCD
        char X=Serial.read();
        switch (X)
        {
          case 'L':  vTaskSuspend(task1_t);
                     break;
          case 'R':  vTaskResume(task1_t);
                     break;
          case 'U':   xSemaphoreGive(Sem_Handle);
                     break;

        }
      }
  }
}   

void loop()
{
}