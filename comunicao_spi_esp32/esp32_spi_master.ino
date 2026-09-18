#include <SPI.h>

const int CS_PIN = 5;
static const uint32_t BUFFER_SIZE = 64;

char tx_buf[BUFFER_SIZE] = {0};
char rx_buf[BUFFER_SIZE] = {0};

void setup() {
  Serial.begin(115200);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  SPI.begin(); // Pinos padrão VSPI (18, 19, 23)
  
  Serial.println("=========================================");
  Serial.println("        SPI CHAT - MODO MASTER           ");
  Serial.println(" Digite sua mensagem no terminal e envie ");
  Serial.println("=========================================");
}

void loop() {
  bool temMensagemParaEnviar = false;

  // 1. Verifica se o usuário digitou algo no PC
  if (Serial.available() > 0) {
    String msgTerminal = Serial.readStringUntil('\n');
    msgTerminal.trim();

    if (msgTerminal.length() > 0) {
      Serial.print("[Você (Master)]: ");
      Serial.println(msgTerminal);
      
      memset(tx_buf, 0, BUFFER_SIZE);
      msgTerminal.toCharArray(tx_buf, BUFFER_SIZE);
      temMensagemParaEnviar = true;
    }
  }

  // Se não houver mensagem nova, enviamos um buffer totalmente zerado (vazio)
  // apenas para dar a oportunidade do Slave falar
  if (!temMensagemParaEnviar && millis() % 200 == 0) { 
    memset(tx_buf, 0, BUFFER_SIZE); 
  }

  // Executa a transação SPI caso tenha mensagem ou no tempo de escuta (polling)
  if (temMensagemParaEnviar || (millis() % 200 == 0)) {
    memset(rx_buf, 0, BUFFER_SIZE); 

    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); // 1 MHz
    digitalWrite(CS_PIN, LOW); // Ativa o Slave

    // Troca de dados síncrona
    for (uint32_t i = 0; i < BUFFER_SIZE; i++) {
      rx_buf[i] = SPI.transfer(tx_buf[i]);
    }

    digitalWrite(CS_PIN, HIGH); // Libera o Slave
    SPI.endTransaction();

    // 2. Se o Slave nos respondeu com algum texto válido, exibe na tela
    if (rx_buf[0] != '\0') {
      Serial.print("[Amigo (Slave)]: ");
      Serial.println(rx_buf);
    }

    // CORREÇÃO: Limpa o buffer de envio do Master IMEDIATAMENTE após a transmissão
    // para evitar que o mesmo dado seja retransmitido na próxima volta do loop
    if (temMensagemParaEnviar) {
      memset(tx_buf, 0, BUFFER_SIZE);
    }
    
    delay(5); 
  }
}
