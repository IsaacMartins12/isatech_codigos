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


// Esse debaixo é para comunicar e ligar um led

/*

#include <Arduino.h>

#define RX2_PIN 16
#define TX2_PIN 17
#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Garante estado inicial desligado

  Serial.println("Receptor UART Inicializado. Aguardando dados...");
}

void loop() {
  if (Serial2.available() > 0) {
    String dadosRecebidos = Serial2.readStringUntil('\n');
    dadosRecebidos.trim(); // Remove '\r', espaços e quebras residuais

    if (dadosRecebidos == "ligar led") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("Led Ligado");
    } else if (dadosRecebidos == "desligar led") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("Led desligado");
    }

    Serial.print("Recebido: ");
    Serial.println(dadosRecebidos);
  }
}







*/
