#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "iot-irrigation-616e8-default-rtdb.firebaseio.com"  //Database link
#define FIREBASE_AUTH "YCeLRtFGGfLOmdwZy9L60leyJfAaDgDqOq7GKeV4"  //Database secrate

#define WIFI_SSID "IoT Irrigation"      //Router name
#define WIFI_PASSWORD "00000000"  //Router password

FirebaseData firebaseData,loadData;
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);


void senW(String field,float value){
 Firebase.setString(firebaseData, "/AnalogOutput/"+field,String((int)value) );
  
}

String load(String field){
if (Firebase.getString(loadData, "/Load/"+field)){
    return loadData.stringData();
  }
  return "0";
}




void dis(String a, String b){

  lcd.setCursor(0,0);  
  lcd.print(a+"                  ");
  
  lcd.setCursor(0,1);  
  lcd.print(b+"                  ");
  Serial.println(a+" "+b);
}



void setup() {

  pinMode(D4,OUTPUT);
  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(A0,INPUT);
  Serial.begin(9600);
  Wire.begin(D2, D1);
  lcd.begin();
  lcd.home();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  dis("IoT Irrigation","Connecting WiFi!");
  
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(D4,0);
    Serial.print(".");
    delay(200);
    digitalWrite(D4,1);
    Serial.print(".");
    delay(200);
    
  }
   dis("WiFi Connected!","");
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Firebase.setString(firebaseData, "/Hello","World" );
  delay(500);
}

void loop() { 
  
  digitalWrite(D6,analogRead(A0)>800);
  senW("Moisture",map(1023-analogRead(A0),0,1023,0,100));
  delay(100);
  String d=load("Fertilizer Pump");
  digitalWrite(D5,d=="1");
  delay(100);
  String sec="WPump:"+String(digitalRead(D6)==true);
  String sec2="MPump:"+String(digitalRead(D5)==true);
 Serial.println("Moisture: "+String(analogRead(A0)));
  dis(("Moisture: "+String(map(1023-analogRead(A0),0,1023,0,100)))+"%",sec+" "+sec2);
  
  
  
  
  
}
