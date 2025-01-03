#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const uint8_t msg_queue_len = 5;
static const int led_pin = 2;
static QueueHandle_t queue1;
static QueueHandle_t queue2;
static TaskHandle_t taska = NULL;
static TaskHandle_t taskb = NULL;
static int t = 0;

void taskA(void *parameters){
 String blinkChecker, serialInput;
 int ledDelay; 
  while(1){
    if(xQueueReceive(queue2, (void *) &blinkChecker, 0) == pdTRUE){
      Serial.println(blinkChecker);
      }
     if(Serial.available() > 0){
      String serialInput = Serial.readString();
      }
     if(serialInput.substring(0,5) == "delay"){
      serialInput = serialInput.substring(7);
      ledDelay = serialInput.toInt();
         if(xQueueSend(queue1, (void *) &ledDelay, 10) != pdTRUE){
            Serial.println("Queue full");
        }else{ Serial.println("Delay sent");}
      }else{
        Serial.println(serialInput);
        }
    vTaskDelay(1000/portTICK_PERIOD_MS);
    }
  }

  void taskB(void *parameters){
     String blinkChecker = "Blinked";
     int lledDelay;
     int count =0;
  while(1){
    if(xQueueReceive(queue1, (void *) &lledDelay, 0) == pdTRUE){
       digitalWrite(led_pin, HIGH);
       vTaskDelay(lledDelay / portTICK_PERIOD_MS);
       count++;
       digitalWrite(led_pin, LOW);
        vTaskDelay(lledDelay / portTICK_PERIOD_MS);
      }
     if(count >= 100){
         if(xQueueSend(queue2, (void *) &blinkChecker, 10) != pdTRUE){
            Serial.println("Queue full");
        }
        count = 0;
      }
    vTaskDelay(1000/portTICK_PERIOD_MS);
    }
    }
void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
  Serial.println();
  Serial.println("--FreeRTOS Queue Demo--- ");
  queue1 = xQueueCreate(msg_queue_len, sizeof(int));
  queue1 = xQueueCreate(msg_queue_len, sizeof(String));

    xTaskCreatePinnedToCore(taskA, 
    "Task A", 
    1024, 
    NULL,
    1,
    NULL,
    app_cpu);
    
    xTaskCreatePinnedToCore(taskB, 
    "Task B",
    1024,
    NULL,
    1,
    NULL, 
    app_cpu);
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
