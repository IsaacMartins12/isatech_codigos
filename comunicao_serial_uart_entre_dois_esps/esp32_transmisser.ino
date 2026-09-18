#include <Arduino.h>

// Usaremos a Serial2 do ESP32 (Pinos padrão: TX=17, RX=16)
#define RX2_PIN 16
#define TX2_PIN 17

void setup() {
  Serial.begin(115200);   // Monitor Serial para o PC
  Serial2.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN); // Comunicação entre ESPs
  Serial.println("Emissor UART Inicializado.");
}

void loop() {
  String mensagem = "Ola do ESP Emissor via UART!";
  
  Serial.print("Enviando: ");
  Serial.println(mensagem);
  
  Serial2.println(mensagem); // Envia para o outro ESP
  
  delay(2000); // Aguarda 2 segundos
}


// ESSA PARTE DO CÓDIGO VAI NO ARDUINO ESCRAVO