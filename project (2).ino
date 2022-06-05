#include<Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#define FIREBASE_HOST "h-back-hackaton-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "hEj9ecAkhJc85ZrbPwPqTv5PJlEB90GezAmLWNiB"
#define WIFI_SSID "CIU"
#define WIFI_PASSWORD "Ciu12345$"
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
 
int minVal=265;
int maxVal=402,n;
 
double x;
double y;
double z;
 
void setup(){
  Serial.begin(9600);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
Wire.begin();
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);
Serial.begin(9600);
Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
n = Firebase.getInt("TestUser/Points");
}
void loop(){
  n = Firebase.getInt("TestUser/Points");
  Serial.print(n);
  bool ison=Firebase.getBool("General/isOn");
  if(ison){
  int leftx = 70;
  int rightx = 95;
Wire.beginTransmission(MPU_addr);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true);
AcX=Wire.read()<<8|Wire.read();
AcY=Wire.read()<<8|Wire.read();
AcZ=Wire.read()<<8|Wire.read();
int xAng = map(AcX,minVal,maxVal,-90,90);
int yAng = map(AcY,minVal,maxVal,-90,90);
int zAng = map(AcZ,minVal,maxVal,-90,90);
 
x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
 
Serial.print("AngleX= ");
Serial.println(x);
 
Serial.print("AngleY= ");
Serial.println(y);
 
Serial.print("AngleZ= ");
Serial.println(z);
Serial.println("-----------------------------------------");;
 if(y>leftx and y<rightx){
  Serial.println("YEAAH!!!!");
  n++;
  Serial.println(n);
  Firebase.setInt("TestUser/Points", n);
  }
    if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  }
}
