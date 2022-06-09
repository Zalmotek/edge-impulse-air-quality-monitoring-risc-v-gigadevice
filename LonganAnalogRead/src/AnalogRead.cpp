#include <Arduino.h>

int MiCs = PA6; //GND= 0, 5V=4095
int MQ5 = PA7; //PA4: GND= 110, 5V=4095
int MQ7 = PB0; //GND= 0, 5V=4095
int MQ3 = PB1; //PA6: GND= 110, 5V=4095 


int valMiCs = 0; 
int valMQ5 = 0;
int valMQ7 = 0; 
int valMQ3 = 0; 

void setup() {
  Serial.begin(115200);          
}

void loop() {
  valMiCs = analogRead(MiCs);  
  valMQ5 = analogRead(MQ5);
  valMQ7 = analogRead(MQ7); 
  valMQ3 = analogRead(MQ3); 
  Serial.print(valMiCs);
  Serial.print(",");
  Serial.print(valMQ5);
  Serial.print(","); 
  Serial.print(valMQ7);
  Serial.print(",");  
  Serial.println(valMQ3);          
}