#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// ===== Wi-Fi =====
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ===== Telegram =====
#define BOT_TOKEN "YOUR_BOT_TOKEN"
#define CHAT_ID "YOUR_CHAT_ID"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// Настройки камеры (для AI-Thinker ESP32-CAM)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Интервал проверки сообщений
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

// Глобальные переменные для отправки фото
camera_fb_t * fb = NULL;
size_t fb_index = 0;

bool moreDataAvailable() {
  return (fb_index < fb->len);
}

uint8_t getNextByte() {
  if (fb_index < fb->len) {
    return fb->buf[fb_index++];
  }
  return 0;
}

void setup() {
  // Отключаем brownout detector для стабильности
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  
  Serial.begin(115200);
  
  // Настройка камеры
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // Для более высокого качества фото (разрешение)
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; // 1600x1200
    config.jpeg_quality = 10; // 10-63 (меньше = лучше качество)
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA; // 800x600
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  
  // Инициализация камеры
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Ошибка инициализации камеры 0x%x", err);
    return;
  }
  
  // Подключение к Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi подключен");
  
  // Настройка безопасности для Telegram
  client.setInsecure();
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    
    while (numNewMessages) {
      Serial.println("Получено сообщение");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    
    Serial.println("Сообщение: " + text);
    
    // Проверка команды
    if (text.equalsIgnoreCase("/фото") || text.equalsIgnoreCase("фото")) {
      bot.sendMessage(chat_id, "Делаю снимок...", "");
      
      // Делаем фото
      fb = esp_camera_fb_get();
      if (!fb) {
        bot.sendMessage(chat_id, "Ошибка: не удалось сделать снимок", "");
        return;
      }
      
      // Сбрасываем индекс для чтения фото
      fb_index = 0;
      
      // Отправляем фото
      String sent = bot.sendPhotoByBinary(chat_id, "image/jpeg", fb->len, 
                                          moreDataAvailable, getNextByte, nullptr, nullptr);
      
      if (sent) {
        Serial.println("Фото отправлено");
      } else {
        Serial.println("Ошибка отправки фото");
      }
      
      // Освобождаем память
      esp_camera_fb_return(fb);
      fb = NULL;
    }
    
    // Добавьте другие команды при необходимости
    else if (text.equalsIgnoreCase("/start")) {
      String welcome = "Привет! Я бот для ESP32-CAM.\n";
      welcome += "Отправьте команду /фото чтобы получить фотографию.";
      bot.sendMessage(chat_id, welcome, "");
    }
  }
}