#define BLYNK_PRINT Serial
#include <math.h>
#include <ESP8266WiFi.h>
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

char ssid[] = "IEM IEDC LAB"; 
char pass[] = "IEMIEDC501";
char auth[] = "5j5ia570b_O1xl-3W2fWsCnw1UHOkHmb"; 
  
BlynkTimer timer;
WiFiClient client;
 
HX711 scale(D5, D6);

float weight;

float calibration_factor = -107325; 
//float calibration_factor1 = -1059525;
//float calibration_factor2 = -429525;
//float calibration_factor3 = -16269525;
float T_weight;

void setup() {
  Serial.begin(115200);
  pinMode(trig, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echo, INPUT);   // Sets the echoPin as an Input

  scale.set_scale();
  scale.tare(); 

  
  long zero_factor = scale.read_average();
  
  Blynk.begin(auth, ssid, pass);
  WiFi.begin(ssid, pass);{
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  delay(2000);
  
  timer.setInterval(1000L, sendSensor);
}
 
void loop(){ 
  timer.run();
  Blynk.run();
}

void sendSensor(){
  digitalWrite(trig, LOW);   // Makes trigPin low
  delayMicroseconds(2); // 2 micro second delay
  digitalWrite(trig, HIGH);  // tigPin high
  delayMicroseconds(10);      // trigPin high for 10 micro seconds
  digitalWrite(trig, LOW);   // trigPin low
  
  duration = pulseIn(echo, HIGH);   //Read echo pin, time in microseconds
  distance = duration * 0.034 / 2;   //Calculating actual/real distance
  
  Serial.print("Distance = ");        //Output distance on arduino serial monitor
  Serial.print(distance);
  Serial.println(" Cm ");
  Blynk.virtualWrite(V2, distance);
  
  distance_m=distance/100;

  scale.set_scale(calibration_factor)
  weight = scale.get_units(5); 
  T_weight=weight;//+weight1;
  
  float bmi =0.0;
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
    //Blynk.notify("Underweight");
    
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
    //Blynk.notify("Normal Weight");
    
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
    //Blynk.notify("Pre-Obesity");

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
    //Blynk.notify("Obesity class I");

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
    //Blynk.notify("Obesity class II");
    
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
    //Blynk.notify("Obesity class III");

    Blynk.virtualWrite(V0,"Obesity");
    Blynk.virtualWrite(V1,"Class III");
  }
  delay(1000);
}
