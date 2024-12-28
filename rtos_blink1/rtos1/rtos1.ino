#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif
//pins
static const int led_pin = 2;
int delayTime = 20;
static TaskHandle_t task1= NULL;
static TaskHandle_t task2= NULL;
void toggleLED(void *parameter){
  while(1){
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    }
  }
  void toggleLED2(void *parameter){
  while(1){
    digitalWrite(led_pin, HIGH);
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
    }
  }
void setup() {
  Serial.begin(300);
  pinMode(led_pin, OUTPUT);

  xTaskCreatePinnedToCore(
      toggleLED,
      "Toggle LED",
      1024,   //stack size
      NULL,
      0, // priority
      &task1, //task handle
      app_cpu);
      
      xTaskCreatePinnedToCore(
      toggleLED2,
      "Toggle LED 2",
      1024,   //stack size
      NULL,
      1, // priority
      &task2, //task handle
      app_cpu);
    
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available() == 0){};
  delayTime = Serial.parseInt();
  vTaskSuspend(task2); 
  vTaskDelay(delayTime / portTICK_PERIOD_MS);
  vTaskResume(task2);
  Serial.print("Updated LED delay to: ");
  Serial.println(delayTime); 
}
