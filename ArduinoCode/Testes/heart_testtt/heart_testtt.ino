#include "Arduino.h"

#define TAM 50

int pulsePin = 34;                 // Pulse Sensor purple wire connected to analog pin 34 , ADC6

// Volatile Variables, used in the interrupt service routine!
float BPM;                   // int that holds raw Analog in 0. updated every 2mS
int Signal;                // holds the incoming raw data
int P;                     // used to find peak in pulse wave, seeded
unsigned long firstTime;
unsigned long lastBeatTime;
unsigned long timeBetween;
bool isBeat = false;
int lastValues[TAM];
int currentIndex = 0; 
int averageBPM;



void setup() {
  Serial.begin(115200);             // we agree to talk fast!
  P = 512;                    // peak at 1/2 the input range of 0..1023
  firstTime = millis();
  lastBeatTime = millis();
  timeBetween = 101;
}

void loop() {

  if((millis() - firstTime) > 200) {
    firstTime = millis();
    heartSensor();
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

  Signal = analogRead(pulsePin) / 4;            // read the Pulse Sensor, bits of ESP32 ADC ch is 4 times larger
  Serial.println(Signal);
  unsigned long currentTime = millis();

  if((currentTime - lastBeatTime) > 100) {
    if (Signal > P) {
      if (isBeat == false) {
        isBeat = true;
        Serial.println("Detectou pico");
      }
    }
    else {
      if (isBeat == true){ //&& (currentTime - lastBeatTime) > 100) {
        isBeat = false;
        Serial.println("Detectou vale");
        
        timeBetween = currentTime - lastBeatTime;
        
        Serial.print("sinal: ");
        Serial.println(Signal);
        Serial.print("time: ");
        Serial.println(timeBetween);
        
        lastBeatTime = currentTime;
        BPM = (60000.0 / timeBetween);
    
        Serial.print("BPM: ");
        Serial.println(BPM);

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
        Serial.print("Average: ");
        Serial.println(averageBPM);
      }
    }
  }

  if ((currentTime - lastBeatTime) > 2000) {
    isBeat = false;
    lastBeatTime = currentTime;
  }
}
