#include <Arduino_FreeRTOS.h>
#include <timers.h>
#include <task.h>

#define mainONE_SHOT_TIMER_PERIOD pdMS_TO_TICKS(3333)
#define mainAUTO_RELOAD_TIMER_PERIOD pdMS_TO_TICKS(500)

TimerHandle_t xAutoReloadTimer, xOneShotTimer;
BaseType_t xTimer1Started, xTimer2Started;
unsigned long runTime;

volatile int counter = 0;

void setup() {
  Serial.begin(9600);

  // Đặt thời điểm bắt đầu chương trình
  runTime = millis();

  // Khởi tạo one-shot timer
  xOneShotTimer = xTimerCreate("OneShot", mainONE_SHOT_TIMER_PERIOD, pdFALSE, 0, prvOneShotTimerCallback);

  // Khởi tạo auto-reload timer
  xAutoReloadTimer = xTimerCreate("AutoReload", mainAUTO_RELOAD_TIMER_PERIOD, pdTRUE, 0, prvAutoReloadTimerCallback);

  // Kiểm tra xem timers có được tạo thành công không
  if ((xOneShotTimer != NULL) && (xAutoReloadTimer != NULL)) {
    // Khởi động one-shot timer
    xTimer1Started = xTimerStart(xOneShotTimer, 0);

    // Khởi động auto-reload timer
    xTimer2Started = xTimerStart(xAutoReloadTimer, 0);

    // Kiểm tra xem timers đã được khởi động thành công không
    if ((xTimer1Started == pdPASS) && (xTimer2Started == pdPASS)) {
      Serial.println("Timers started");

      // Bắt đầu scheduler
      vTaskStartScheduler();
    }
  }
}

void loop() {
  // Kiểm tra xem đã đủ thời gian chạy chưa
  if (millis() - runTime >= 10000) {
    stopTimers();
  }
}

static void prvOneShotTimerCallback(TimerHandle_t xTimer) {
  Serial.print("One-shot timer callback executing ");
  Serial.println(++counter);
}

static void prvAutoReloadTimerCallback(TimerHandle_t xTimer) {
  Serial.print("Auto-reload timer callback executing ");
  Serial.println(++counter);
}

void stopTimers() {
  // Dừng one-shot timer
  if (xOneShotTimer != NULL && xTimer1Started == pdPASS) {
    xTimerStop(xOneShotTimer, 0);
    xTimer1Started = pdFALSE;
  }

  // Dừng auto-reload timer
  if (xAutoReloadTimer != NULL && xTimer2Started == pdPASS) {
    xTimerStop(xAutoReloadTimer, 0);
    xTimer2Started = pdFALSE;
  }

  // In ra Serial Monitor khi dừng
  Serial.println("Timers stopped");

  // Dừng chương trình
  while (true) {
    // Vòng lặp trống, chương trình sẽ dừng ở đây
  }
}
