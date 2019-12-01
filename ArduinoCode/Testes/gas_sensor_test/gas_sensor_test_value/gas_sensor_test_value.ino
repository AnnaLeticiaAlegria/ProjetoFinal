/* 
 MQ9 
 modified on 19 Feb 2019 
 by Saeed Hosseini 
 https://electropeak.com/learn/ 
*/ 
#define GAS_SENSOR 15

void setup() { 
 Serial.begin(115200); 
} 
void loop() { 
 float sensor_volt; 
 float RS_gas; 
 float ratio; 
//-Replace the name "R0" with the value of R0 in the demo of First Test -/ 
 float R0 = 0.21; 
 float R0_new = 0.98;
 float sensorValue = analogRead(GAS_SENSOR)/4.0; 
 sensor_volt = (sensorValue / 1024) * 10.0; 
  RS_gas = (10.0 - sensor_volt) / sensor_volt; // Depend on RL on yor module 
 ratio = RS_gas / R0; // ratio = RS/R0 
//------------------------------------------------------------/ 
 Serial.print("sensor_volt = "); 
 Serial.println(sensor_volt); 
 Serial.print("RS_ratio = "); 
 Serial.println(RS_gas); 
 Serial.print("Rs/R0 = "); 
 Serial.println(ratio); 
 Serial.print("Rs/R0_new = "); 
 Serial.println(RS_gas/R0_new); 
 Serial.print("\n\n"); 
 
 delay(1000); 
}
