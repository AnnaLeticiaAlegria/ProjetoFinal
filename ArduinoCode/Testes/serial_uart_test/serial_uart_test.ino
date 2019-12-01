#include <SoftwareSerial.h>


SoftwareSerial mySoftwareSerial(A9, A8); // RX, TX

uart_config_t uart_config = {
  .baud_rate = 115200,
  .data_bits = UART_DATA_8_BITS,
  .parity    = UART_PARITY_DISABLE,
  .stop_bits = UART_STOP_BITS_1,
  .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
};

void setup() {
  // put your setup code here, to run once:
  mySoftwareSerial.begin(115200);

  Serial.begin(115200);
}

void loop() {
 if (Serial.available() > 0) {
    //char incomingByte = Serial.read();
    Serial.write("testando serial \r");
  }
  delay(10);    
}
