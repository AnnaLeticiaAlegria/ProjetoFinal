
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

unsigned long fallTimer = 0;
int fallCount = 0;

float lastX = 0;
float lastY = 0;
float lastZ = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  mpu6050.update();

  if(millis() - fallTimer > 700){
    
//    Serial.println("=======================================================");
//    Serial.print("temp : ");Serial.println(mpu6050.getTemp());
    float currentX = mpu6050.getAccX();
    float currentY = mpu6050.getAccY();
    float currentZ = mpu6050.getAccZ();
    fallTimer = millis();

//    Serial.print(lastX - currentX);
    if (fallCount == 1) {
      Serial.println(abs(lastX - currentX));
       Serial.println(abs(lastY - currentY));
        Serial.println(abs(lastZ - currentZ));
      if(abs(lastY - currentY) < 0.25 && abs(lastZ - currentZ) < 0.25) {
        Serial.print("accX : ");Serial.println(currentX);
        Serial.print("gyroX : ");Serial.print(mpu6050.getGyroX());
        Serial.print("accAngleX : ");Serial.print(mpu6050.getAccAngleX());
        Serial.print("gyroAngleX : ");Serial.print(mpu6050.getGyroAngleX());
        Serial.print("angleX : ");Serial.print(mpu6050.getAngleX());
        Serial.println(" .   CAIUUUUUUUU");
      }
      else {
        fallCount = 0;
      }
    }
    if (lastX - currentX > 0.4) {
      fallCount = 1;
      Serial.print("fallCount : ");
      Serial.println(fallCount);
    }
    lastX = currentX;
    lastY = currentY;
    lastZ = currentZ;
  }

}
