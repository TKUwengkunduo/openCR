#include "string.h"

int temp = 0;
int o=0;
int u1 = 1;
int u2 = 2;
int u3 = 3;
int u4 = 5;


void setup() {
  Serial.begin(9600);

}

void loop() {
  while(!Serial.available()){}
  temp = Serial.readString().toInt();

  if(temp==23){
    Serial.println(u1);
  }else if(temp==24){
    Serial.println(u2);
  }else if(temp==25){
    Serial.println(u3);
  }else{
    Serial.println(temp);
  }
  
  
  
}
