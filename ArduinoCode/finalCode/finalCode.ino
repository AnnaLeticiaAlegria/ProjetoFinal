#define GAS_SENSOR 15
#define VIBRATION_MOTOR  25
#define HEART_PULSE 34

/* Heart sensor declarations */
#define TAM 50
float BPM;
int heartSignal;
int P;
unsigned long firstTime;
unsigned long lastBeatTime;
unsigned long timeBetween;
bool isBeat = false;
int lastValues[TAM];
int currentIndex = 0; 
int averageBPM;

/* Gas sensor declarations */
float sensorVolt;
float RS;
float gasRatio;
float R0;
float sensorValue;
unsigned long motorTime;
int number = 0;
int alertState = 0;

/* Serial definitions */
unsigned long serialTime;

/* Sensors definitions */
bool isGasOn = true;
bool isMotorOn = true;
bool isHeartOn = true;
bool isFallOn = true;
 
void setup() { 

  /* Motor definitions */
  pinMode(VIBRATION_MOTOR, OUTPUT );
  digitalWrite(VIBRATION_MOTOR, LOW);

  /* Serial definitions */
  serialTime = millis();
  Serial.begin(115200);
  Serial.setTimeout(500);
  Serial2.begin(115200);
  Serial2.setTimeout(500);

  /* Heart sensor definitions */
  pinMode(HEART_PULSE, INPUT );
  P = 512;
  firstTime = millis();
  lastBeatTime = millis();
  timeBetween = 101;
  
  /* Gas sensor definitions */
  R0 = 0.1;
 
} 

void alert(unsigned long currentTime) {
  if (currentTime - motorTime > 500) {
    digitalWrite(VIBRATION_MOTOR, alertState);
    alertState = !alertState;
    motorTime = currentTime;
  }
}

int calculateAverage(int * vector, int n) {
  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum += vector[i];
  }
  return sum / n;
}

void heartSensor () {

  heartSignal = analogRead(HEART_PULSE) / 4;            // read the Pulse Sensor, bits of ESP32 ADC ch is 4 times larger
  unsigned long currentTime = millis();

  if((currentTime - lastBeatTime) > 100) {
    if (heartSignal > P) {
      if (isBeat == false) {
        isBeat = true;
      }
    }
    else {
      if (isBeat == true){
        isBeat = false;
        
        timeBetween = currentTime - lastBeatTime;
        
        lastBeatTime = currentTime;
        BPM = (60000.0 / timeBetween);

        if (currentIndex < TAM) {
          lastValues[currentIndex] = BPM;
          currentIndex ++;
        }
        else {
          for (int i=0;i<TAM - 1;i++) {
            lastValues[i] = lastValues[i+1];
          }
          lastValues[TAM-1] = BPM;
        }

        averageBPM = calculateAverage(lastValues, currentIndex);
      }
    }
  }

  if ((currentTime - lastBeatTime) > 2000) {
    isBeat = false;
    lastBeatTime = currentTime;
  }
}
void loop() { 
  
  bool gasAlert = false;
  unsigned long currentTime = millis();

  if (isGasOn) {
    /* Gas sensor calcs */
    sensorValue = analogRead(GAS_SENSOR)/4.0; 
    sensorVolt = (sensorValue / 1024) * 10.0; 
    RS = (10.0 - sensorVolt) / sensorVolt; // Depend on RL on yor module 
    gasRatio = RS / R0; // ratio = RS/R0 
  }

  if (isMotorOn) {  
    /* Motor ifs */
    if (gasRatio < 9.40) {
      alert (currentTime);
      Serial.print("gasSensor: ");
      Serial.println(gasRatio);
      gasAlert = true;
    }
    else {
      digitalWrite(VIBRATION_MOTOR, LOW);
    }
  }

  if (isHeartOn) {
    /* Heart Sensor */
    if((currentTime - firstTime) > 200) {
      firstTime = currentTime;
      heartSensor();
    }
  }

  /* Send serial */
  if (currentTime - serialTime > 4000) {

    if (isGasOn) {   
      if (gasAlert) {
        Serial2.print("gas==true//");
        Serial.print("gas==true//");
      }
      else {  
        Serial2.print("gas==false//");
        Serial.print("gas==false//");
      }
    }

    if (isHeartOn && averageBPM != 0) {
      Serial2.print("heart==" + String(averageBPM) + "//");
      Serial.print("heart==" + String(averageBPM) + "//");
    }
    
    Serial2.print("\r");
    Serial.print("\r");
    serialTime = currentTime;
  }
 

  delay(10); 
}
