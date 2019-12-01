
const int OUTPUT_TYPE = SERIAL_PLOTTER;

const int PIN_INPUT = 34;
const int PIN_BLINK = 13;    // Pin 13 is the on-board LED
const int THRESHOLD = 550;   // Adjust this number to avoid noise when idle

/*
   All the PulseSensor Playground functions.
*/
PulseSensorPlayground pulseSensor;

void setup() {
  /*
     Use 115200 baud because that's what the Processing Sketch expects to read,
     and because that speed provides about 11 bytes per millisecond.

     If we used a slower baud rate, we'd likely write bytes faster than
     they can be transmitted, which would mess up the timing
     of readSensor() calls, which would make the pulse measurement
     not work properly.
  */
  Serial.begin(115200);

  // Configure the PulseSensor manager.

  pulseSensor.analogInput(PIN_INPUT);
  pulseSensor.blinkOnPulse(PIN_BLINK);
  pulseSensor.fadeOnPulse(PIN_FADE);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);
  pulseSensor.setThreshold(THRESHOLD);

  // Now that everything is ready, start reading the PulseSensor signal.
  if (!pulseSensor.begin()) {
    /*
       PulseSensor initialization failed,
       likely because our particular Arduino platform interrupts
       aren't supported yet.

       If your Sketch hangs here, try ProcessEverySample.ino,
       which doesn't use interrupts.
    */
    for(;;) {
      // Flash the led to show things didn't work.
      digitalWrite(PIN_BLINK, LOW);
      delay(50);
      digitalWrite(PIN_BLINK, HIGH);
      delay(50);
    }
  }
}

void loop() {
  /*
     Wait a bit.
     We don't output every sample, because our baud rate
     won't support that much I/O.
  */
  delay(20);

  // write the latest sample to Serial.
 pulseSensor.outputSample();

  /*
     If a beat has happened since we last checked,
     write the per-beat information to Serial.
   */
  if (pulseSensor.sawStartOfBeat()) {
   pulseSensor.outputBeat();
  }
}
