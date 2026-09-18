#include <Wire.h>

#define I2C_DEV_ADDR 0x55
static const uint32_t BUFFER_SIZE = 32;

// Atributos voláteis porque são alterados dentro das interrupções por hardware
volatile char tx_buf[BUFFER_SIZE] = {0};
volatile bool temDadosParaEnviar = false;

// Callback: Executado automaticamente quando o Master ENVIA dados para o Slave
void receberDados(int quantosBytes) {
  String msgRecebida = "";
  while (Wire.available()) {
    char c = Wire.read();
    if (c != '\0') msgRecebida += c;
  }
  
  msgRecebida.trim();
  if (msgRecebida.length() > 0) {
    Serial.print("[Amigo (Master)]: ");
    Serial.println(msgRecebida);
  }
}

// Callback: Executado automaticamente quando o Master PEDE dados para o Slave
void responderDados() {
  if (temDadosParaEnviar) {
    Wire.write((const uint8_t*)tx_buf, BUFFER_SIZE);
    
    // Limpa o buffer após o envio bem-sucedido
    memset((void*)tx_buf, 0, BUFFER_SIZE);
    temDadosParaEnviar = false;
  } else {
    // Se não tiver o que falar, responde vazio para não travar o barramento
    Wire.write('\0'); 
  }
}

void setup() {
  Serial.begin(115200);

  // Assina as funções de evento antes de iniciar o barramento
  Wire.onReceive(receberDados);
  Wire.onRequest(responderDados);

  // Inicializa o I2C passando o endereço (Torna-se Slave automaticamente)
  Wire.begin(I2C_DEV_ADDR); 

  Serial.println("=========================================");
  Serial.println("         I2C CHAT - MODO SLAVE           ");
  Serial.println(" Digite sua mensagem no terminal e envie ");
  Serial.println("=========================================");
}

void loop() {
  // 1. Monitora o terminal do PC para coletar a resposta do Slave
  if (Serial.available() > 0 && !temDadosParaEnviar) {
    String msgTerminal = Serial.readStringUntil('\n');
    msgTerminal.trim();

    if (msgTerminal.length() > 0) {
      Serial.print("[Você (Slave)]: ");
      Serial.println(msgTerminal);

      // Prepara o buffer para a próxima requisição do Master
      memset((void*)tx_buf, 0, BUFFER_SIZE);
      msgTerminal.toCharArray((char*)tx_buf, BUFFER_SIZE);
      temDadosParaEnviar = true; 
    }
  }
}
