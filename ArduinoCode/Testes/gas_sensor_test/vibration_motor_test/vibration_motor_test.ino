#define VIBRATION_MOTOR  32
 
void setup()  
{
    pinMode(VIBRATION_MOTOR, OUTPUT );
}
 
void loop()  
{
    digitalWrite(VIBRATION_MOTOR, HIGH);
    delay(1000);
    digitalWrite(VIBRATION_MOTOR, LOW);
    delay(1000);
}
