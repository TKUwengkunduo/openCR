/*******************************************************************************
  Copyright 2016 ROBOTIS CO., LTD.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*******************************************************************************/

/* Authors: Taehun Lim (Darby) */

#include <DynamixelWorkbench.h>
//#include <random.h>

#define DXL_BUS_SERIAL3 "3"            //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#define BAUDRATE  1000000
#define R_Motor_ID    1
#define L_Motor_ID    2
//#define 
//#define KP             0.8
#define KI             0
#define SERIAL Serial2
float KP[6]={0.7, 1.1, 1.1, 1.1, 0.7, 1};
int target_speed[6] = {100, 100, 100, 100 ,110, 0};
int error[50]={0};
int  R_speed=0;
int L_speed=0;
int temp=0;
int jetson_sign;
int sign = 48;

DynamixelWorkbench R_Motor;
DynamixelWorkbench L_Motor;



void setup()
{
  Serial.begin(9600);
  SERIAL.begin(9600);
  Serial.setTimeout(100);

  R_Motor.begin(DXL_BUS_SERIAL3, BAUDRATE);
  L_Motor.begin(DXL_BUS_SERIAL3, BAUDRATE);

  R_Motor.ping(R_Motor_ID);
  L_Motor.ping(L_Motor_ID);

  R_Motor.wheelMode(R_Motor_ID);
  L_Motor.wheelMode(L_Motor_ID);
}



void loop()
{
  
  jetson_sign = SERIAL.read();
  for(int i=0;i<30;i++){
    SERIAL.read();
  }
//  Serial.print("jetson_sign - sign= ");
//  Serial.println(jetson_sign - sign);
  if(jetson_sign != -1){
    if(jetson_sign - sign == 4){
      for(int i=0;i<=65;i++){
        left_line();
        SERIAL.read();
      }
      stop_car();
      delay(1000);
      for(int i=0;i<=45;i++){
        error[0] = Serial.readString().toInt();
        double_yellow_line();
//        SERIAL.read();
      }
      for(int i=0;i<=50;i++){
        left_line();
        SERIAL.read();
      }
    }
    else if(jetson_sign - sign == 1){
      stop_car();

      delay(500);

      for(int i=0;i<=105;i++){
        left_line();
      }
    }
    else if(jetson_sign - sign == 2){
      stop_car();
      
      delay(500);

      for(int i=0;i<=105;i++){
        right_line();

      }
    }
    else if(jetson_sign - sign == 3){
      delay(500);
      stop_car();
      delay(500);
      for(int i=0;i<=85;i++){
        Avoidance();
        SERIAL.read();
      }
    }
    else if(jetson_sign - sign == 5){
      SERIAL.read();
      stop_car();
      delay(1000);
      while(SERIAL.read()==5){
        stop_car();
        delay(1000);
      }
    }
    else{
      double_line();
    }
  }
  else{
    double_line();
  }
    
//  Avoidance();
//  left_line();

}



int PID_conrtoller(char RL, int mode){
  float wheel_speed=0;
  float P_pid=0, I_pid=0;
  int error_sum=0;
  int i;

//  for(i=0;i<50;i++){
//    error_sum=error_sum+error[i];
//  }
  
  P_pid = (error[0])*KP[mode];
//  Serial.print("P=");
//  Serial.println(error[0]);
//  I_pid = error_sum*KI;
  

  if(RL=='R'){
    wheel_speed = int(target_speed[mode]+P_pid);
  }else{
    wheel_speed = int(target_speed[mode]-P_pid);
  }
  
  

  return wheel_speed;
}


void go_straight(){
  R_speed = 100;
  L_speed = 100;
  R_Motor.goalSpeed(R_Motor_ID, -R_speed);
  L_Motor.goalSpeed(L_Motor_ID, -L_speed);
  delay(3500);
}
void turn_left(){
  R_speed = 80;
  L_speed = -80;
  R_Motor.goalSpeed(R_Motor_ID, -R_speed);
  L_Motor.goalSpeed(L_Motor_ID, -L_speed);
  delay(2000);
}

void turn_around(){
  R_speed = 80;
  L_speed = -80;
  R_Motor.goalSpeed(R_Motor_ID, -R_speed);
  L_Motor.goalSpeed(L_Motor_ID, -L_speed);
  delay(4000);
}

void turn_right(){
  R_speed = -80;
  L_speed = 80;
  R_Motor.goalSpeed(R_Motor_ID, -R_speed);
  L_Motor.goalSpeed(L_Motor_ID, -L_speed);
  delay(2000);
}

void double_line(){
  Serial.print('0');
  
  error[0] = Serial.readString().toInt();
  if(error[0]==0){
    R_speed = R_speed;
    L_speed = R_speed;
    R_Motor.goalSpeed(R_Motor_ID, -R_speed);
    L_Motor.goalSpeed(L_Motor_ID, -L_speed);
  }else if(error[0]==-999){
    R_Motor.goalSpeed(R_Motor_ID, 0);
    L_Motor.goalSpeed(L_Motor_ID, 0);
  }else{
    R_speed = PID_conrtoller('R', 0);
    L_speed = PID_conrtoller('L', 0);
    R_Motor.goalSpeed(R_Motor_ID, -R_speed);
    L_Motor.goalSpeed(L_Motor_ID, -L_speed);
  }
}

void left_line(){
  // mode
  Serial.print('1');
  
  error[0] = Serial.readString().toInt();

  if(error[0]==0){
    R_speed = R_speed;
    L_speed = R_speed;
    R_Motor.goalSpeed(R_Motor_ID, -R_speed);
    L_Motor.goalSpeed(L_Motor_ID, -L_speed);
  }else if(error[0]==-999){
    R_Motor.goalSpeed(R_Motor_ID, 0);
    L_Motor.goalSpeed(L_Motor_ID, 0);
  }else{
    R_speed = PID_conrtoller('R', 1);
    L_speed = PID_conrtoller('L', 1);
    R_Motor.goalSpeed(R_Motor_ID, -R_speed);
    L_Motor.goalSpeed(L_Motor_ID, -L_speed);
//    Serial.print(R_speed);
//    Serial.print('|');
//    Serial.println(L_speed);
  }
}

void right_line(){
  // mode
  Serial.print('2');
  
  error[0] = Serial.readString().toInt();

  if(error[0]==0){
    R_speed = R_speed;
    L_speed = R_speed;
    R_Motor.goalSpeed(R_Motor_ID, -R_speed);
    L_Motor.goalSpeed(L_Motor_ID, -L_speed);
  }else if(error[0]==-999){
    R_Motor.goalSpeed(R_Motor_ID, 0);
    L_Motor.goalSpeed(L_Motor_ID, 0);
  }else{
    R_speed = PID_conrtoller('R', 2);
    L_speed = PID_conrtoller('L', 2);
    R_Motor.goalSpeed(R_Motor_ID, -R_speed);
    L_Motor.goalSpeed(L_Motor_ID, -L_speed);
//    Serial.print(R_speed);
//    Serial.print('|');
//    Serial.println(L_speed);
  }
  
}


void double_yellow_line(){
  // mode
  Serial.print('4');
  
  error[0] = Serial.readString().toInt();
  if(error[0]==0){
    R_speed = R_speed;
    L_speed = R_speed;
    R_Motor.goalSpeed(R_Motor_ID, -R_speed);
    L_Motor.goalSpeed(L_Motor_ID, -L_speed);
  }
  if(error[0]==-777){
    stop_car();
    for(int i=0;i<=5;i++){
      Serial.print('9');
      delay(200);
      error[0] = Serial.readString().toInt();
    }
    do{
      error[0] = Serial.readString().toInt();
    }while(error[0]!=-77 && error[0]!=-7);
    Serial.print('4');
    if(error[0]==-77){
      go_straight();
      turn_right();
      go_straight();
      turn_around();
      go_straight();
      turn_left();
      go_straight();
      delay(700);
    }
    else{
      go_straight();
      turn_left();
      go_straight();
      turn_around();
      go_straight();
      turn_right();
      go_straight();
      delay(700);
    }
    do{
      error[0] = Serial.readString().toInt();
    }while(error[0]==-77 && error[0]==-7 && error[0]==-777);
  }
  else if(error[0]==-999){
    stop_car();
  }else{
    R_speed = PID_conrtoller('R', 4);
    L_speed = PID_conrtoller('L', 4);
    R_Motor.goalSpeed(R_Motor_ID, -R_speed);
    L_Motor.goalSpeed(L_Motor_ID, -L_speed);
//    Serial.print(R_speed);
//    Serial.print('|');
//    Serial.println(L_speed);
  }
}
void Avoidance(){
  // mode
  Serial.print('3');
  
  error[0] = Serial.readString().toInt();

  if(error[0]==0){
    R_speed = R_speed;
    L_speed = R_speed;
    R_Motor.goalSpeed(R_Motor_ID, -R_speed);
    L_Motor.goalSpeed(L_Motor_ID, -L_speed);
  }else if(error[0]==-999){
    R_Motor.goalSpeed(R_Motor_ID, 0);
    L_Motor.goalSpeed(L_Motor_ID, 0);
  }else{
    R_speed = PID_conrtoller('R', 3);
    L_speed = PID_conrtoller('L', 3);
    R_Motor.goalSpeed(R_Motor_ID, -R_speed);
    L_Motor.goalSpeed(L_Motor_ID, -L_speed);
//    Serial.print("KP=");
//    Serial.println(target_speed[3]);
//    Serial.print("speed= ");
//    Serial.print(R_speed);
//    Serial.print('|');
//    Serial.println(L_speed);

  }
  
}

void stop_car(){
  R_speed = 0;
  L_speed = 0;
  R_Motor.goalSpeed(R_Motor_ID, -R_speed);
  L_Motor.goalSpeed(L_Motor_ID, -L_speed);
}
