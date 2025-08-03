#include <SPI.h> // Подключаем библиотеку SPI

#include <nRF24L01.h> // Подключаем библиотеку nRF24L01

#include <RF24.h> // Подключаем библиотеку RF24

RF24 radio(9,10); // Указываем номера выводов nRF24L01: CE и CSN

const uint32_t pipe = 0x1234567890LL; // адрес "трубы" - адрес приёмника

void setup() {

while (!Serial); // для Leonardo и т. п. плат

// ждёт пока откроем монитор порта

Serial.begin(9600); // устанавливаем скор. обмена данными 9600 бод

radio.begin(); // Инициируем работу nRF24L01

radio.setChannel(0x6f); // Устанавливаем канал

radio.openReadingPipe(0, pipe); // Открываем "трубу" чтения

radio.startListening(); // начинаем прослушивать

}

void loop() {

if (radio.available()) { // если модуль получил информацию

char text[32] = {0}; // переменная, хранящая текст сообщения

radio.read(&text, sizeof(text)); // читаем текст сообщения

Serial.println(text); // выводим текст в монитор порта

}

}