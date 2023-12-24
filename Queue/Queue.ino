//Thuvien
  //RTC
  #include "RTClib.h"
  #include <Adafruit_Sensor.h>
  //DHT
  #include "DHT.h"
  //FastLED
  #include <FastLED.h>
  //LCD
  #include <LiquidCrystal_I2C.h>
//Define
  //Define RTC
    RTC_DS1307 rtc;
    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    DateTime now;
  //Define DHT11
    #define DHTPIN  19     // Digital pin connected to the DHT sensor
    #define DHTTYPE DHT11   
    DHT dht(DHTPIN, DHTTYPE);
  //Define FastLED
    #define pin 4  // dinh nghia 1 hang so, o day pin la chan so 6
    #define num 8   // num la so led, o day la 8
    CRGB leds[num];//tao mang leds 
  //Define Buzzer
    #define NOTE_B0 31
    #define NOTE_C1 33
    #define NOTE_CS1 35
    #define NOTE_D1 37
    #define NOTE_DS1 39
    #define NOTE_E1 41
    #define NOTE_F1 44
    #define NOTE_FS1 46
    #define NOTE_G1 49
    #define NOTE_GS1 52
    #define NOTE_A1 55
    #define NOTE_AS1 58
    #define NOTE_B1 62
    #define NOTE_C2 65
    #define NOTE_CS2 69
    #define NOTE_D2 73
    #define NOTE_DS2 78
    #define NOTE_E2 82
    #define NOTE_F2 87
    #define NOTE_FS2 93
    #define NOTE_G2 98
    #define NOTE_GS2 104
    #define NOTE_A2 110
    #define NOTE_AS2 117
    #define NOTE_B2 123
    #define NOTE_C3 131
    #define NOTE_CS3 139
    #define NOTE_D3 147
    #define NOTE_DS3 156
    #define NOTE_E3 165
    #define NOTE_F3 175
    #define NOTE_FS3 185
    #define NOTE_G3 196
    #define NOTE_GS3 208
    #define NOTE_A3 220
    #define NOTE_AS3 233
    #define NOTE_B3 247
    #define NOTE_C4 262
    #define NOTE_CS4 277
    #define NOTE_D4 294
    #define NOTE_DS4 311
    #define NOTE_E4 330
    #define NOTE_F4 349
    #define NOTE_FS4 370
    #define NOTE_G4 392
    #define NOTE_GS4 415
    #define NOTE_A4 440
    #define NOTE_AS4 466
    #define NOTE_B4 494
    #define NOTE_C5 523
    #define NOTE_CS5 554
    #define NOTE_D5 587
    #define NOTE_DS5 622
    #define NOTE_E5 659
    #define NOTE_F5 698
    #define NOTE_FS5 740
    #define NOTE_G5 784
    #define NOTE_GS5 831
    #define NOTE_A5 880
    #define NOTE_AS5 932
    #define NOTE_B5 988
    #define NOTE_C6 1047
    #define NOTE_CS6 1109
    #define NOTE_D6 1175
    #define NOTE_DS6 1245
    #define NOTE_E6 1319
    #define NOTE_F6 1397
    #define NOTE_FS6 1480
    #define NOTE_G6 1568
    #define NOTE_GS6 1661
    #define NOTE_A6 1760
    #define NOTE_AS6 1865
    #define NOTE_B6 1976
    #define NOTE_C7 2093
    #define NOTE_CS7 2217
    #define NOTE_D7 2349
    #define NOTE_DS7 2489
    #define NOTE_E7 2637
    #define NOTE_F7 2794
    #define NOTE_FS7 2960
    #define NOTE_G7 3136
    #define NOTE_GS7 3322
    #define NOTE_A7 3520
    #define NOTE_AS7 3729
    #define NOTE_B7 3951
    #define NOTE_C8 4186
    #define NOTE_CS8 4435
    #define NOTE_D8 4699
    #define NOTE_DS8 4978
    #define NOTE_REST 0
  //Define LCD
    LiquidCrystal_I2C lcd(0x27,16,2);
  //Define SerialRead
    String key;
QueueHandle_t data;
SemaphoreHandle_t Sem;
void setup(){
  Serial.begin(9600);
  khaibao_rtc();
  khaibao_dht();
  khaibao_buzzer();
  khaibao_lcd();
  FastLED.addLeds<WS2812, pin, GRB>(leds, num); 
  
  data = xQueueCreate(4,sizeof(int));
  Sem = xSemaphoreCreateBinary();

  xTaskCreate(RTC,"RTC Task",4000,NULL,2,NULL);
  xTaskCreate(DHT,"DHT Task",4000,NULL,3,NULL);
  xTaskCreate(fastLED,"FastLED Task",4000,NULL,2,NULL);
  xTaskCreate(BUZZER,"BUZZER Task",4000,NULL,3,NULL);
  xTaskCreate(LCD,"LCD Task",4000,NULL,3,NULL);
  xTaskCreate(SerialRead,"Serial Task",4000,NULL,3,NULL);
}
void loop(){}

//Func SerialRead
  void SerialRead(void*){
    int n, r;
    while(1){
      Serial.println("Serial is running");
      char S = Serial.read();
      key = "";
      key += S;
      if( key == "S"){
        if(xQueueSend(data,&n,pdMS_TO_TICKS(2000)) ==  pdPASS){;
          Serial.print("Send Data to Queue: "); Serial.println(n);
          Serial.print("Queue Free: "); Serial.print(uxQueueSpacesAvailable(data)); Serial.println(" spaces");
          n = 0;
        }
        else{
          Serial.println("ERROR: Failed to Send Data to queue...!");
        }
      }
      if(key == "N" ){     
        n++;
        Serial.print(" n = ");
        Serial.println(n);
      }
      if(key == "L" ){     
        xSemaphoreGive(Sem);
        Serial.println("Give Sem for FastLED");
      }
      if(key == "R" ){     
        if(xQueueReceive(data,&r,pdMS_TO_TICKS(2000))==pdPASS){
          Serial.print("Receive Data: "); Serial.println(r);
          Serial.print("Queue Free: "); Serial.print(uxQueueSpacesAvailable(data)); Serial.println(" spaces");
        }
        else{
        Serial.println("ERROR: Failed to Received Data from queue---!");
        }
      }
    vTaskDelay(pdMS_TO_TICKS(400));
    }
  }
//Func RTC
  void khaibao_rtc(){

    if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      Serial.flush();
      while (1) 
      delay(10);
    }

    if (!rtc.isrunning()) {
      Serial.println("RTC is NOT running, let's set the time!");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0)); cau hinh thu cong
    }
  }
  void RTC(void*){
    while(1){
      Serial.print("RTC is running.....");  
      DateTime now = rtc.now();
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(" (");
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
      Serial.print(") ");
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.println(now.second(), DEC);
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
//Func DHT11
  void khaibao_dht(){
    dht.begin();
  }
  void DHT(void*){
    while(1){
    Serial.print("DHT task is running----");
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.println(F("°C "));
    vTaskDelay(pdMS_TO_TICKS(3000));
    }
  }
//Func FastLED
  void fastLED(void*){
    int times;
    while(1){
    xSemaphoreTake(Sem, portMAX_DELAY);
    Serial.println("FastLED success TakeSemaphore !!!");
    xQueueReceive(data,&times,portMAX_DELAY);
    Serial.print("FastLED run: "); Serial.print(times); Serial.println(" times...");
    Serial.print("Queue Free: "); Serial.println(uxQueueSpacesAvailable(data)); Serial.print(" spaces");
    for(int l = 0; l < times; l++) {
      Serial.println("FastLED is running--------------------------");
      for(int i = 0; i < num; i++) { 
                leds[i] = CRGB::Blue;
                FastLED.show();
                // clear this led for the next time around the loop
                leds[i] = CRGB::Black;
                vTaskDelay(pdMS_TO_TICKS(50));
            }
      for(int i = 0; i < num; i++) { 
                leds[i] = CRGB::Red;
                FastLED.show();
                // clear this led for the next time around the loop
                leds[i] = CRGB::Black;
                vTaskDelay(pdMS_TO_TICKS(50));
            }
      for(int i = 0; i < num; i++) { 
                leds[i] = CRGB::Yellow;
                FastLED.show();
                // clear this led for the next time around the loop
                leds[i] = CRGB::Black;
                vTaskDelay(pdMS_TO_TICKS(50));
            }
                leds[7] = CRGB::Black;
                FastLED.show();
      }
    }
  }
//Func buzzer
  int melody[] = {
    // Các giá trị nốt nhạc
    NOTE_FS5,  NOTE_E5, NOTE_A5, NOTE_FS5, NOTE_E5, NOTE_A4, NOTE_E5, NOTE_D5, NOTE_CS5, NOTE_D5, NOTE_FS5, NOTE_D5, NOTE_CS5, NOTE_B4, NOTE_REST,
    NOTE_B4, NOTE_G5, NOTE_FS5, NOTE_E5, NOTE_CS5, NOTE_A4, NOTE_CS5, NOTE_E5, NOTE_FS5, NOTE_REST,
    NOTE_A4, NOTE_FS5, NOTE_E5, NOTE_D5, NOTE_CS5, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_REST,
    NOTE_G4, NOTE_G5, NOTE_FS5, NOTE_E5, NOTE_CS5, NOTE_A4, NOTE_CS5, NOTE_A5, NOTE_FS5, NOTE_E5, NOTE_D5, NOTE_REST,
  };

  int noteDurations[] = {
    // Thời gian của từng nốt nhạc
    1, 2, 1, 4, 3, 2, 1, 4, 3, 3, 2, 4, 3, 2, 4,
    3, 4, 4, 4, 2, 2, 3, 2, 1, 4,
    3, 4, 4, 4, 2, 3, 3, 3, 1,
    3, 4, 4, 4, 2, 3, 2, 3, 1, 4, 4, 2,
  };
  int buzzerPin = 26; // Chân kết nối buzzer

  void khaibao_buzzer(){
    pinMode(buzzerPin, OUTPUT);
  }
  void BUZZER(void*){
    while(1){
      Serial.println("Buzzer is running-------");
    for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
      int noteDuration = 600 / noteDurations[i];
      if (melody[i] != NOTE_REST) {
        tone(buzzerPin, melody[i], noteDuration);
      }
      vTaskDelay(pdMS_TO_TICKS(noteDuration));
      noTone(buzzerPin);
    }
    }
  }
//Func LCD
  void khaibao_lcd(){
    lcd.init();                      // initialize the lcd 
    // Print a message to the LCD.
    lcd.backlight();
    lcd.setCursor(0,0);
  }
  void LCD(void*){
      //LCD
      while(1){
        DateTime now = rtc.now();
        int h = dht.readHumidity();
        int t = dht.readTemperature();
        Serial.println("LCD is running");
          lcd.setCursor(0,0);
          lcd.print(now.hour(), DEC);
          lcd.print(':');
          lcd.print(now.minute(), DEC);
          lcd.print(':');
          lcd.print(now.second(), DEC);
          lcd.print("   ");
          lcd.print(now.day(), DEC);
          lcd.print('/');
          lcd.print(now.month(), DEC);
        lcd.setCursor(0,1);
          lcd.print(F("H:"));
          lcd.print(h);
          lcd.print(F("% T:"));
          lcd.print(t);
          lcd.print(F("°C "));
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
  }