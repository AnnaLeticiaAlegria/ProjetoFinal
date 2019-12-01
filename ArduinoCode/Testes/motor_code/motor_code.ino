#define VIBRATION_MOTOR  32

unsigned long motorTime;
int number = 0;
int alertState = 0;

void alert(unsigned long currentTime) {
  if (currentTime - motorTime > 500) {
    digitalWrite(VIBRATION_MOTOR, alertState);
    alertState = !alertState;
    motorTime = currentTime;
  }
}
 
void setup()  
{
    pinMode(VIBRATION_MOTOR, OUTPUT );
    Serial.begin(115200);
    
    digitalWrite(VIBRATION_MOTOR, LOW);
}
 
void loop()  
{
//    digitalWrite(VIBRATION_MOTOR, HIGH);
//    delay(1000);
//    digitalWrite(VIBRATION_MOTOR, LOW);
//    delay(1000);
  unsigned long currentTime = millis();
  number ++;
  if (number > 200) {
    alert (currentTime);
  }
  else {
    digitalWrite(VIBRATION_MOTOR, LOW);
  }

  if(number > 500) {
    number = 0;
  }
  Serial.println(number);
  delay(10);
}
