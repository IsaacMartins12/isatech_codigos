#include <Arduino.h>

// Definição dos pinos da Serial2 (Comunicação entre os ESPs)
#define RX2_PIN 16
#define TX2_PIN 17

void setup() {
  // Inicializa o Monitor Serial do computador (USB)
  Serial.begin(115200);   
  
  // Inicializa a comunicação direta entre os ESP32
  Serial2.begin(115200, SERIAL_8N1, RX2_PIN, TX2_PIN); 

  Serial.println("=========================================");
  Serial.println("          CHAT ESP32 INICIALIZADO        ");
  Serial.println(" Digite sua mensagem no terminal e envie ");
  Serial.println("=========================================");
}

void loop() {
  // 1. LER DO MONITOR SERIAL (PC) E ENVIAR PARA O OUTRO ESP32
  if (Serial.available() > 0) {
    // Lê o que você digitou no computador até apertar 'Enter'
    String mensagemParaEnviar = Serial.readStringUntil('\n');
    mensagemParaEnviar.trim(); // Remove espaços extras ou quebras de linha em branco

    if (mensagemParaEnviar.length() > 0) {
      // Exibe no seu próprio monitor o que você acabou de mandar
      Serial.print("[Você]: ");
      Serial.println(mensagemParaEnviar);

      // Envia o texto via hardware para o outro ESP32
      Serial2.println(mensagemParaEnviar);
    }
  }

  // 2. RECEBER DO OUTRO ESP32 E EXIBIR NO MONITOR SERIAL (PC)
  if (Serial2.available() > 0) {
    // Lê a string que veio do outro ESP32
    String mensagemRecebida = Serial2.readStringUntil('\n');
    mensagemRecebida.trim();

    if (mensagemRecebida.length() > 0) {
      // Exibe na tela a mensagem que chegou do parceiro
      Serial.print("[Amigo]: ");
      Serial.println(mensagemRecebida);
    }
  }
}
