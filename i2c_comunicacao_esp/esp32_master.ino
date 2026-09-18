#include <Wire.h>

#define SLAVE_ADDR 0x55 // Endereço do ESP32 Slave
static const uint32_t BUFFER_SIZE = 32;

char tx_buf[BUFFER_SIZE] = {0};

void setup() {
  Serial.begin(115200);
  
  // Inicializa o I2C no modo Master (Pinos padrão: SDA=21, SCL=22)
  Wire.begin(); 
  
  Serial.println("=========================================");
  Serial.println("        I2C CHAT - MODO MASTER           ");
  Serial.println(" Digite sua mensagem no terminal e envie ");
  Serial.println("=========================================");
}

void loop() {
  // 1. Enviar do Master para o Slave
  if (Serial.available() > 0) {
    String msgTerminal = Serial.readStringUntil('\n');
    msgTerminal.trim();

    if (msgTerminal.length() > 0) {
      Serial.print("[Você (Master)]: ");
      Serial.println(msgTerminal);

      // Inicia transmissão para o endereço do Slave
      Wire.beginTransmission(SLAVE_ADDR);
      
      // CORREÇÃO AQUI: Passando os bytes e o tamanho exato da mensagem de forma limpa
      Wire.write((const uint8_t*)msgTerminal.c_str(), msgTerminal.length());
      
      Wire.endTransmission();
    }
  }

  // 2. Pedir dados para o Slave (Polling periódico a cada 200ms)
  static unsigned long tempoAnterior = 0;
  if (millis() - tempoAnterior >= 200) {
    tempoAnterior = millis();

    // Solicita os bytes ao Slave
    uint8_t bytesRecebidos = Wire.requestFrom(SLAVE_ADDR, BUFFER_SIZE);
    
    if (bytesRecebidos > 0) {
      String msgRecebida = "";
      while (Wire.available()) {
        char c = Wire.read();
        if (c != '\0' && c != '\xFF') { // Filtra caracteres nulos/vazios
          msgRecebida += c;
        }
      }
      
      msgRecebida.trim();
      if (msgRecebida.length() > 0) {
        Serial.print("[Amigo (Slave)]: ");
        Serial.println(msgRecebida);
      }
    }
  }
}
