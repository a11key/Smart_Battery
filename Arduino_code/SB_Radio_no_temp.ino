// NRF24L01+ PA/LNA https://microkontroller.ru/arduino-projects/kak-rabotaet-modul-nrf24l01-i-kak-ego-podklyuchit-k-arduino/?ysclid=m4thxl154l853962068
#include <SPI.h>                                          // Подключаем библиотеку для работы с шиной SPI
#include <nRF24L01.h>                                     // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>                                         // Подключаем библиотеку для работы с nRF24L01+

RF24           radio(9, 10);                              // Создаём объект radio для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN) 

//T
#include <Wire.h>
#include "Adafruit_MCP9808.h"

// // Create the MCP9808 temperature sensor objects
// Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
// Adafruit_MCP9808 tempsensor_2 = Adafruit_MCP9808();

const int V_Pin = A1;     // voltage pin
double V = 0; // voltage value

const int currentPin = A0; // current pin
double I = 0; // current value
double sensitivity = 0.066; // for 30 A
float offsetVoltage = 2.5;


double data[4];

void setup() {
  Serial.begin(9600);  

  // NRF24L01+ PA/LNA
  radio.begin();                                        // Инициируем работу nRF24L01+
    radio.setChannel(126);                                  // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
    radio.setDataRate     (RF24_2MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
    radio.setPALevel      (RF24_PA_MAX);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    radio.openWritingPipe (0x1234567890LL);               // Открываем трубу с идентификатором 0x1234567890 для передачи данных (на одном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
    pinMode(2, INPUT);
    radio.setAutoAck(false);       // Отключите подтверждение
    radio.setRetries(0, 0);  
                                

  // T
  //  if (!tempsensor.begin(0x18)) {
  // Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct. 1");
  // while (1);
  // }
  // if (!tempsensor_2.begin(0x19)) {
  // Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct. 2");
  // while (1);
  // }
    
  //  Serial.println("Found MCP9808!");

  // // tempsensor.setResolution(3);
  // tempsensor_2.setResolution(0); // sets the resolution mode of reading, the modes are defined in the table bellow:
  // // Mode Resolution SampleTime
  // //  0    0.5°C       30 ms
  // //  1    0.25°C      65 ms
  // //  2    0.125°C     130 ms
  // //  3    0.0625°C    250 ms



}

void loop() {
  // unsigned long start_time = micros();
  
  // // // Замер времени чтения температуры
  // // tempsensor_2.wake();
  // // double T2 = tempsensor_2.readTempC();
  // // tempsensor_2.shutdown_wake(1);
  // unsigned long temp_time = micros();
  
  double R1 = 9860;
  double R2 = 11970;
  double R3 = 11890;
  // Замер времени чтения напряжения
  V = analogRead(V_Pin) * (5.0 / 1024.0) * (1 + ((R2 + R3) / R1));
  // unsigned long voltage_time = micros();
  
  // Замер времени чтения тока
  I = (((analogRead(currentPin) / 1024.0) * 5.0) - offsetVoltage) / sensitivity - 0.368;
  // unsigned long current_time = micros();
  
  // Замер времени отправки по радио
  data[0] = V;
  data[1] = I;
  data[2] = 0;
  data[3] = 0;
  radio.write(&data, sizeof(data));
  // unsigned long radio_time = micros();
  
  // Замер времени вывода в Serial
  for (int i = 0; i < 4; i++) Serial.println(data[i]);
  unsigned long serial_time = micros();
  
  // Вывод результатов замеров
  // Serial.print("Timing (us): Temp=");
  // Serial.print(temp_time - start_time);
  // Serial.print(" Volt=");
  // Serial.print(voltage_time - temp_time);
  // Serial.print(" Curr=");
  // Serial.print(current_time - voltage_time);
  // Serial.print(" Radio=");
  // Serial.print(radio_time - current_time);
  // Serial.print(" Serial=");
  // Serial.println(serial_time - radio_time);
  
  // delay(300); 
}
