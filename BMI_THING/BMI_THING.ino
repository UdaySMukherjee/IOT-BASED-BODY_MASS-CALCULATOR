
#include <math.h>
#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" 
#include <SPI.h>
#include "HX711.h"
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>

#define  trig  D0
#define  echo  D1

long duration;
float distance; 
float distance_m;
float bmi =0.0;
  
char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password;
char auth[] = "5j5ia570b_O1xl-3W2fWsCnw1UHOkHmb"; 
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

BlynkTimer timer;
WiFiClient client;
 
HX711 scale;
float weight;
float calibration_factor = -107325; 
float T_weight;

void setup() {
  Serial.begin(115200);
  pinMode(trig, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echo, INPUT);// Sets the echoPin as an Input
  pinMode(D2, OUTPUT);
  scale.begin(D5, D6);
  scale.set_scale();
  scale.tare(); 
  digitalWrite(D2,LOW);
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  long zero_factor = scale.read_average();
  Blynk.begin(auth, ssid, pass);
  WiFi.begin(ssid, pass);{
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
   if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  delay(2000);
  timer.setInterval(1000L, sendSensor);
}
 
void loop(){ 
  timer.run();
  Blynk.run();
  
  int send_data=digitalRead(D2);
  if(send_data==HIGH){
    int x = ThingSpeak.writeField(myChannelNumber, 1, bmi, myWriteAPIKey); //data sent to thingspeak cloud
  }
}

void sendSensor(){
  digitalWrite(trig, LOW);   // Makes trigPin low
  delayMicroseconds(2); // 2 micro second delay
  digitalWrite(trig, HIGH);  // tigPin high
  delayMicroseconds(10);      // trigPin high for 10 micro seconds
  digitalWrite(trig, LOW);   // trigPin low
  duration = pulseIn(echo, HIGH);   //Read echo pin, time in microseconds
  distance = duration * 0.034 / 2;   //Calculating actual/real distance
  if(distance>500){
    distance=0.0;
  }
  if(distance>=0.0 && distance<=200){
  Serial.print("Distance = ");    
  Serial.print(distance);
  Serial.println(" Cm ");
  Blynk.virtualWrite(V2, distance);
  }
  distance_m=distance/100;

  scale.set_scale(calibration_factor);
  weight = scale.get_units(5); 
  T_weight=weight;
  

  bmi= T_weight/(distance_m * distance_m);
  
  if(bmi < 18.5){
    Blynk.virtualWrite(V3, fabs(T_weight));
    Serial.print("Weight: ");
    Serial.print(fabs(T_weight));
    Serial.println(" KG");
    Serial.println();
    Serial.println(bmi);
    Serial.println("Underweight");
    Blynk.virtualWrite(V4 , bmi);
    Blynk.virtualWrite(V0,"Under");
    Blynk.virtualWrite(V1,"Weight");
  }
  
  else if(bmi >= 18.5 && bmi <= 24.9){
    Blynk.virtualWrite(V3, fabs(T_weight));
    Serial.print("Weight: ");
    Serial.print(fabs(T_weight));
    Serial.println(" KG");
    Serial.println();
    Serial.println(bmi);
    Serial.println("Normal Weight");
    Blynk.virtualWrite(V4 , bmi);
    Blynk.virtualWrite(V0,"Normal");
    Blynk.virtualWrite(V1,"Weight");
  }
  
  else if(bmi >= 25.0 && bmi <= 29.9){
    Blynk.virtualWrite(V3, fabs(T_weight));
    Serial.print("Weight: ");
    Serial.print(fabs(T_weight));
    Serial.println(" KG");
    Serial.println();
    Serial.println(bmi);
    Serial.println("Pre-Obesity");
    Blynk.virtualWrite(V4 , bmi);
    Blynk.virtualWrite(V0,"Pre");
    Blynk.virtualWrite(V1,"Obesity");
  }
  
  else if(bmi >= 30.0 && bmi <= 34.9){
    Blynk.virtualWrite(V3, fabs(T_weight));
    Serial.print("Weight: ");
    Serial.print(fabs(T_weight));
    Serial.println(" KG");
    Serial.println();
    Serial.println(bmi);
    Serial.println("Obesity class I");
    Blynk.virtualWrite(V4 , bmi);
    Blynk.virtualWrite(V0,"Obesity");
    Blynk.virtualWrite(V1,"Class I");
  }
  
  else if(bmi >= 35.0 && bmi <= 39.9){
    Blynk.virtualWrite(V3, fabs(T_weight));
    Serial.print("Weight: ");
    Serial.print(fabs(T_weight));
    Serial.println(" KG");
    Serial.println();
    Serial.println(bmi);
    Serial.println("Obesity class II");
    Blynk.virtualWrite(V4 , bmi);
    Blynk.virtualWrite(V0,"Obesity");
    Blynk.virtualWrite(V1,"Class II");
  }
  
  else if(bmi >= 40){
    Blynk.virtualWrite(V3, fabs(T_weight));
    Serial.print("Weight: ");
    Serial.print(fabs(T_weight));
    Serial.println(" KG");
    Serial.println();
    Serial.println(bmi);
    Serial.println("Obesity class III");
    Blynk.virtualWrite(V4 , bmi);
    Blynk.virtualWrite(V0,"Obesity");
    Blynk.virtualWrite(V1,"Class III");
  }
  delay(1000);
}
