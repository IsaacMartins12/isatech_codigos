#include <Arduino.h>
#include <driver/spi_slave.h> // Driver oficial e nativo do ESP32

static const uint32_t BUFFER_SIZE = 64;

// Buffers alinhados na memória (exigência do hardware do ESP32 para SPI)
DMA_ATTR uint8_t tx_buf[BUFFER_SIZE] = {0};
DMA_ATTR uint8_t rx_buf[BUFFER_SIZE] = {0};

bool temDadosFila = false;

void setup() {
  Serial.begin(115200);

  // Configuração dos pinos do canal VSPI do ESP32
  spi_bus_config_t buscfg = {
    .mosi_io_num = 23,
    .miso_io_num = 19,
    .sclk_io_num = 18,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 0
  };

  // Configuração das propriedades do Slave
  spi_slave_interface_config_t slvcfg = {
    .spics_io_num = 5,
    .flags = 0,
    .queue_size = 3,
    .mode = SPI_MODE0,
    .post_setup_cb = NULL,
    .post_trans_cb = NULL
  };

  // Inicializa o hardware SPI no modo Slave usando DMA (canal 2)
  spi_slave_initialize(VSPI_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);

  Serial.println("=========================================");
  Serial.println("    SPI CHAT NATIVO - MODO SLAVE         ");
  Serial.println(" Digite sua mensagem no terminal e envie ");
  Serial.println("=========================================");
}

void loop() {
  // 1. Verifica se o usuário digitou algo no PC
  if (Serial.available() > 0 && !temDadosFila) {
    String msgTerminal = Serial.readStringUntil('\n');
    msgTerminal.trim();

    if (msgTerminal.length() > 0) {
      Serial.print("[Você (Slave)]: ");
      Serial.println(msgTerminal);

      memset(tx_buf, 0, BUFFER_SIZE);
      memcpy(tx_buf, msgTerminal.c_str(), msgTerminal.length());
      temDadosFila = true; 
    }
  }

  // Configura a transação que vai aguardar o comando do Master
  spi_slave_transaction_t t;
  memset(&t, 0, sizeof(t));
  t.length = BUFFER_SIZE * 8; // Tamanho em bits
  t.tx_buffer = tx_buf;
  t.rx_buffer = rx_buf;

  // O ESP32 pausa aqui nesta linha até o Master enviar o Clock e o CS_PIN baixar
  // O "portMAX_DELAY" faz ele esperar o tempo que for necessário sem travar o chip
  esp_err_t ret = spi_slave_transmit(VSPI_HOST, &t, portMAX_DELAY);

  if (ret == ESP_OK) {
    // Se o primeiro byte não for nulo, significa que o Master mandou texto
    if (rx_buf[0] != '\0') {
      Serial.print("[Amigo (Master)]: ");
      
      String msgRecebida = "";
      for (int i = 0; i < BUFFER_SIZE; i++) {
        if (rx_buf[i] == '\0') break;
        msgRecebida += (char)rx_buf[i];
      }
      Serial.println(msgRecebida);
    }

    // Se transmitiu com sucesso os dados que colocamos na fila, limpa o buffer
    if (temDadosFila) {
      memset(tx_buf, 0, BUFFER_SIZE);
      temDadosFila = false;
    }

    // Limpa o buffer de recepção para a próxima rodada
    memset(rx_buf, 0, BUFFER_SIZE);
  }
}
