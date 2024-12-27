#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif
//pins
static const int led_pin = 2;
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
    vTaskDelay(300 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    }
  }
void setup() {
  pinMode(led_pin, OUTPUT);

  xTaskCreatePinnedToCore(
      toggleLED,
      "Toggle LED",
      1024,   //stack size
      NULL,
      1, // priority
      NULL, //task handle
      app_cpu);
      
      xTaskCreatePinnedToCore(
      toggleLED2,
      "Toggle LED 2",
      1024,   //stack size
      NULL,
      1, // priority
      NULL, //task handle
      app_cpu);
    
}

void loop() {
  // put your main code here, to run repeatedly:

}
