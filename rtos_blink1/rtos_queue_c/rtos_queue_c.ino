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
static const uint8_t buf_len = 255;
static const char command[] = "delay ";


void taskA(void *parameters){
 String blinkChecker, serialInput;
 char c;
 char buf[bef_len];
 uint8_t bufpos = 0;
 uint8_t cmd_len = strlen(command);
 int ledDelay; 

 memset(buf, 0,buf_len);
  while(1){
    if(xQueueReceive(queue2, (void *) &blinkChecker, 0) == pdTRUE){
      Serial.println(blinkChecker);
      }
     if(Serial.available() > 0){
      c = Serial.read();
      if(bufpos < buf_len-1){
        buf[bufpos] = c;
        bufpos++;
        }

        if((c == '\n') || (c == '\r')){
          Serial.print("\r\n");
          if(memcmp(buf, command, cmd_len) == 0){
              char* tail = buf + cmd_len;
              ledDelay = atoi(tail);
              ledDelay = abs(ledDelay);

              if(xQueueSend(queue1, (void *) &ledDelay, 10) != pdTRUE){
            Serial.println("Queue full");
              }else{ Serial.println("Delay sent");}
            }
            memset(buf, 0, buf_len);
            bufpos = 0;
          }else{
            Serial.print(c);
            }
      }
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
