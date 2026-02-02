# ESP32-CAM Telegram Photo Bot 📷

This project allows an **ESP32-CAM (AI-Thinker)** module to capture a photo and send it directly to Telegram when the user sends the command **/фото**.

The ESP32-CAM connects to Wi-Fi, listens for new Telegram messages, and automatically responds with an image whenever the correct command is received.

---

## 🚀 Features

- ESP32-CAM photo capture through Telegram  
- Works with AI-Thinker ESP32-CAM module  
- Sends JPEG images directly to a Telegram chat  
- Simple commands support:
  - `/start` — shows instructions  
  - `/фото` or `фото` — takes and sends a picture  

---

## 🛠 Requirements

### Hardware
- ESP32-CAM (AI-Thinker)
- USB-to-TTL programmer (FTDI / CP2102 / CH340)

### Arduino Libraries
Install these libraries in Arduino IDE (Library Manager):

- **UniversalTelegramBot** (v1.5.0)
- **ArduinoJson** (v6.21.0)

---

## ⚙ Setup Instructions

### Step 1 — Create a Telegram Bot

1. Open Telegram  
2. Search for **BotFather**  
3. Send:


4. Choose a name and username  
5. Copy your bot token, for example: 123456789:ABCDEFxxxxxxxxxxxxxxxxxxxx
 
---

### Step 2 — Configure the Code

Open the ESP32-CAM sketch and replace these lines with your own Wi-Fi and bot token:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

#define BOT_TOKEN "YOUR_BOT_TOKEN"
