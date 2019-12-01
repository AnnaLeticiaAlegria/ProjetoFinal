#include "Arduino.h"

int pulsePin = 34;                 // Pulse Sensor purple wire connected to analog pin 34 , ADC6

// Volatile Variables, used in the interrupt service routine!
int BPM;                   // int that holds raw Analog in 0. updated every 2mS
int Signal;                // holds the incoming raw data
unsigned long IBI = 600;             // int that holds the time interval between beats! Must be seeded!
bool Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".

int rate[10];                    // array to hold last ten IBI values
unsigned long sampleCounter = 0;          // used to determine pulse timing
unsigned long lastBeatTime = 0;           // used to find IBI
int P = 512;                     // used to find peak in pulse wave, seeded
int T = 512;                     // used to find trough in pulse wave, seeded
int thresh = 530;                // used to find instant moment of heart beat, seeded
int amp = 0;                   // used to hold amplitude of pulse waveform, seeded
bool firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
bool secondBeat = false;      // used to seed rate array so we startup with reasonable BPM

unsigned long firstBeatTime = 0;
unsigned long secondBeatTime = 0;
int currentRate = 0;

unsigned long firstTime;


void setup() {
  Serial.begin(115200);             // we agree to talk fast!

  firstTime = millis();
}

void loop() {

  if((millis() - firstTime) > 20) {
    firstTime = millis();
    heartSensor();
  }

//  serialOutput() ;
}

void serialOutput(){   // Decide How To Output Serial.
  Serial.print(BPM);
  Serial.print(",");
  Serial.print(IBI);
  Serial.print(",");
  Serial.println(Signal);
}


void heartSensor () {

  Signal = analogRead(pulsePin) / 4;            // read the Pulse Sensor, bits of ESP32 ADC ch is 4 times larger

  sampleCounter += 2;                         // keep track of the time in mS with this variable
  int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise

  //  find the peak and trough of the pulse wave
  if (Signal < thresh && N > (IBI / 5) * 3) { // avoid dichrotic noise by waiting 3/5 of last IBI
    if (Signal < T) {                       // T is the trough
      T = Signal;                         // keep track of lowest point in pulse wave
      Serial.println("T redefinido");
    }
  }

  if (Signal > thresh && Signal > P) {        // thresh condition helps avoid noise
    P = Signal;                             // P is the peak
    Serial.println("P redefinido");
  }                                        // keep track of highest point in pulse wave

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (N > 250) {                                  // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI / 5) * 3) ) {
      Pulse = true;                               // set the Pulse flag when we think there is a pulse
      lastBeatTime = sampleCounter;               // keep track of time for next pulse

      if (secondBeat) {                      // if this is the second beat, if secondBeat == TRUE
        Serial.println("firstBeat");
        secondBeat = false;                  // clear secondBeat flag
        IBI = millis() - firstBeatTime;
        Serial.println(IBI);
        rate[currentRate] = IBI;
        currentRate = (currentRate + 1)%10;
      }

      if (firstBeat) {                       // if it's the first time we found a beat, if firstBeat == TRUE
        
        Serial.println("firstBeat");
        firstBeat = false;                   // clear firstBeat flag
        secondBeat = true;                   // set the second beat flag
        firstBeatTime = millis();
        return;                              // IBI value is unreliable so discard it
      }

      Serial.println("Antes bpm");
      // keep a running total of the last 10 IBI values
      unsigned int runningTotal = 0;                  // clear the runningTotal variable

      for (int i = 0; i < 10; i++) {          // shift data in the rate array
        runningTotal += rate[i];              // add up the 9 oldest IBI values
      }

      runningTotal /= 10;                     // average the last 10 IBI values
      BPM = 60000 / runningTotal;             // how many beats can fit into a minute? that's BPM!
      Serial.println("BPM redefinido");
      Serial.println(BPM);
      Serial.println(runningTotal);
    }
  }

  if (Signal < thresh && Pulse == true) {  // when the values are going down, the beat is over
    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp / 2 + T;                  // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
    T = thresh;
  }

  if (N > 2500) {                          // if 2.5 seconds go by without a beat
    thresh = 530;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                    // when we get the heartbeat back
  }

}
