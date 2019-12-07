#include <MPU6050_tockn.h>
#include <Wire.h>

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
bool highGas = false;

/* Serial declarations */
unsigned long serialTime;

/* Sensors declarations */
bool isGasOn = true;
bool isAlertOn = true;
bool isHeartOn = true;
bool isFallOn = true;

/* Accelerometer declarations */
MPU6050 mpu6050(Wire);
unsigned long fallTimer = 0;
int fallCount = 0;
float lastX = 0;
float lastY = 0;
float lastZ = 0;
bool fallDetected = false;

/* Alert declarations */
bool alertSent = false;
int alertTimes = 0;

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

  /* Accelerometer definitions */
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void alert(unsigned long currentTime, unsigned long duration) {
  if (currentTime - motorTime > duration) {
    digitalWrite(VIBRATION_MOTOR, alertState);
    alertState = !alertState;
    motorTime = currentTime;
    alertTimes ++;
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

  if ((currentTime - lastBeatTime) > 100) {
    if (heartSignal > P) {
      if (isBeat == false) {
        isBeat = true;
      }
    }
    else {
      if (isBeat == true) {
        isBeat = false;

        timeBetween = currentTime - lastBeatTime;

        lastBeatTime = currentTime;
        BPM = (60000.0 / timeBetween);

        if (currentIndex < TAM) {
          lastValues[currentIndex] = BPM;
          currentIndex ++;
        }
        else {
          for (int i = 0; i < TAM - 1; i++) {
            lastValues[i] = lastValues[i + 1];
          }
          lastValues[TAM - 1] = BPM;
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

void fallDetection(unsigned long currentTime) {
  mpu6050.update();

  if(currentTime - fallTimer > 200){
    float currentX = mpu6050.getAccX();
    float currentY = mpu6050.getAccY();
    float currentZ = mpu6050.getAccZ();
    fallTimer = currentTime;

    if (fallCount == 1) {
      if(abs(lastY - currentY) < 0.25 && abs(lastZ - currentZ) < 0.25) {
        fallDetected = true;
      }
      else {
        fallCount = 0;
        fallDetected = false;
      }
    }
    if (lastX - currentX > 0.4) {
      fallCount = 1;
    }
    lastX = currentX;
    lastY = currentY;
    lastZ = currentZ;
  }

}

void loop() {

  bool gasAlert = false;
  unsigned long currentTime = millis();

  if (isGasOn) {
    /* Gas sensor calcs */
    sensorValue = analogRead(GAS_SENSOR) / 4.0;
    sensorVolt = (sensorValue / 1024) * 10.0;
    RS = (10.0 - sensorVolt) / sensorVolt; // Depend on RL on yor module
    gasRatio = RS / R0; // ratio = RS/R0

    if (gasRatio < 8) {
      highGas = true;
      alert (currentTime, 500);
      Serial.print("gasSensor: ");
      Serial.println(gasRatio);
      gasAlert = true;
    }
    else {
      if (!alertSent) {
        digitalWrite(VIBRATION_MOTOR, LOW);
      }
      highGas = false;
    }
  }

  if (isHeartOn) {
    /* Heart Sensor */
    if ((currentTime - firstTime) > 200) {
      firstTime = currentTime;
      heartSensor();
    }
  }

  if (isFallOn) {
    /* Accelerometer fall detection */
    fallDetection(currentTime);
  }

  if (isAlertOn) {
    /* Alert handler */
    if (alertSent) {
      if (alertTimes < 7) {
        alert(currentTime, 1000);
      }
      else {
        alertSent = false;
        alertTimes = 0;
        if (!highGas) {
          digitalWrite(VIBRATION_MOTOR, LOW);
        }
      }
    }
  }

  /* Send serial */
  if (currentTime - serialTime > 4000) {

    if (isGasOn) {
      if (gasAlert) {
        Serial2.print("gas==true//");
      }
      else {
        Serial2.print("gas==false//");
      }
    }

    if (isHeartOn && averageBPM > 50) {
      Serial2.print("heart==" + String(averageBPM) + "//");
    }

    if (isFallOn) {
      if (fallDetected) {
        Serial2.print("fall==true//");
      }
      else {
        Serial2.print("fall==false//");
      }
    }

    Serial2.print("\r");
    serialTime = currentTime;
  }

  /* Receive serial */
  String receivedText = Serial2.readString();
  if (receivedText != "") {
    receivedText.trim();
    Serial.println(receivedText);
    if (receivedText.startsWith("*")) {
      int idx1 = receivedText.indexOf("==");
      String topic = receivedText.substring(1, idx1); //removes "*"
      String dataR = receivedText.substring(idx1 + 2, receivedText.length() - 2);
      if(topic == "gas") {
        if (dataR == "false") isGasOn = false;
        else isGasOn = true;
      }
      else if(topic == "alert") {
        if (dataR == "false") isAlertOn = false;
        else isAlertOn = true;
      }
      else if(topic == "heart") {
        if (dataR == "false") isHeartOn = false;
        else isHeartOn = true;
      }
      else if(topic == "fall") {
        if (dataR == "false") isFallOn = false;
        else isFallOn = true;
      }
      Serial.println(topic + " --> " + dataR);
    }
    if (receivedText.startsWith("+")) {
      int idx1 = receivedText.indexOf("==");
      String topic = receivedText.substring(1, idx1); //removes "+"
      String dataR = receivedText.substring(idx1 + 2, receivedText.length() - 2);
      if(topic == "alert") {
        if(isAlertOn) {
          alertSent = true;
          alertTimes = 0;
          alert(currentTime, 1000);
        }
      }
    }
  }

  delay(10);
}
