#define GAS_SENSOR 15

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(GAS_SENSOR, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  int gas_sensor_state = analogRead(GAS_SENSOR);
  Serial.print("Gas data: ");
  Serial.println(gas_sensor_state);
  delay(10);
}
