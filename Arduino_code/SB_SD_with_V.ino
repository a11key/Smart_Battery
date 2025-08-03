#include <SPI.h>
#include <SD.h>

#define INS 5      // Пин для определения наличия карты
#define ANALOG_PIN A7  // Пин, с которого считывается напряжение

int fileIndex = 1;
String fileName = "";
File dataFile;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(INS, INPUT_PULLUP);
  Serial.println("Waiting for the card to be inserted");
  while (digitalRead(INS));
  delay(1000);

  if (!SD.begin()) {
    Serial.println("Initialization failed");
    while (true);
  }

  Serial.println("Card OK");

  // Поиск свободного имени файла log_1.csv, log_2.csv и т.д.
  while (true) {
    fileName = "log_" + String(fileIndex) + ".csv";
    if (!SD.exists(fileName)) {
      break;
    }
    fileIndex++;
  }

  dataFile = SD.open(fileName, FILE_WRITE);
  if (dataFile) {
    dataFile.println("Time_ms,Voltage_V");  // Заголовок CSV
    dataFile.close();
    Serial.print("File created: ");
    Serial.println(fileName);
  } else {
    Serial.println("Error creating file");
    while (true);
  }
}

void loop() {
  // Считываем время и напряжение
  unsigned long currentTime = millis() / 1000.0;  // Время в миллисекундах с момента запуска
  int raw = analogRead(ANALOG_PIN);      // Значение от 0 до 1023

  float pin_voltage = raw * (5.0 / 1023.0);  // Перевод в вольты (предположим, опорное напряжение 5В)
  float voltage = pin_voltage * 2.377;
  // Открываем файл и записываем данные
  dataFile = SD.open(fileName, FILE_WRITE);
  if (dataFile) {
    dataFile.print(currentTime);
    dataFile.print(",");
    dataFile.println(voltage, 3);  // 3 знака после запятой
    dataFile.close();
  } else {
    Serial.println("Ошибка записи файла");
  }

  delay(100);  // Ждём 1 секунду между измерениями
}
