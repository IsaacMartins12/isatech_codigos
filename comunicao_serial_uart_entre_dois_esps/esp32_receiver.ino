#include <Arduino.h>

#define RX2_PIN 16
#define TX2_PIN 17

void setup() {
  Serial.begin(115200); // Monitor Serial para o PC
  Serial2.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN); // Comunicação entre ESPs
  Serial.println("Receptor UART Inicializado. Aguardando dados...");
}

void loop() {
  // Verifica se há dados vindos do outro ESP
  if (Serial2.available() > 0) {
    String dadosRecebidos = Serial2.readStringUntil('\n');
    
    Serial.print("Recebido com sucesso: ");
    Serial.println(dadosRecebidos);
  }
}
