//HardwareSerial Serial1(2);

unsigned long serialTime;

void setup() {
  serialTime = millis();
  Serial.begin(115200);
  Serial.setTimeout(500);
  Serial2.begin(115200);
  Serial2.setTimeout(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  int BPM = 78;
  unsigned long currentTime = millis();

  if (currentTime - serialTime > 4000) {
//    if (Serial2.available()){
      Serial2.print("heart==68//gas==false//fall==true//\r");
      Serial.print("heart==68//gas==false//fall==true//\r");
//    }
    serialTime = currentTime;
  }
}
