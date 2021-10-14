
#include<Wire.h> // This library allows you to communicate with I2C devices.
const int MPU=0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;// variables for accelerometer raw data
const int LaserPin = 10; // Laser Pin 10
int LaserOFF = 30; // Value for Laser OFF Straight Wrist
int LaserON = 1000; // Value for Laser ON Wrist Down

void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU);// Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);

  AcZ=Wire.read()<<8|Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

  // print out data
  Serial.print("Accelerometer: ");
  Serial.print("Z = "); Serial.println(AcZ); 
  
  Serial.print("Gyroscope: ");
  Serial.print(" Z = "); Serial.println(GyZ);
  Serial.println(" ");
  pinMode(LaserPin, OUTPUT); // Laser Ready
  digitalWrite(LaserPin, LOW); // Laser is off
  
}
void loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false);// the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU,12,true);  // request a total of 12 registers
  AcZ=Wire.read()<<8|Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  
  Serial.print("Accelerometer: ");
  Serial.print("Z = "); Serial.println(AcZ); 
  
  Serial.print("Gyroscope: ");
  Serial.print(" Z = "); Serial.println(GyZ);
  Serial.println(" ");
  delay(3330);

  if (AcZ < LaserOFF){ // Laser is off Straight Wrist
  digitalWrite(10, LOW);
  delay(100);
  }
  
  if (AcZ > LaserON){ // Laser is on Wrist is down
  digitalWrite(10, HIGH);
  delay(100);
  }

   delay(100);
}
