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

#define DXL_BUS_SERIAL3 "3"            //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#define BAUDRATE  1000000
#define R_Motor_ID    1
#define L_Motor_ID    2

DynamixelWorkbench R_Motor;
DynamixelWorkbench L_Motor;

void setup()
{
  Serial.begin(1000000);

  R_Motor.begin(DXL_BUS_SERIAL3, BAUDRATE);
  L_Motor.begin(DXL_BUS_SERIAL3, BAUDRATE);

  R_Motor.ping(R_Motor_ID);
  L_Motor.ping(L_Motor_ID);

  R_Motor.wheelMode(R_Motor_ID);
  L_Motor.wheelMode(L_Motor_ID);
}

void loop()
{
  R_Motor.goalSpeed(R_Motor_ID, 30);
  L_Motor.goalSpeed(L_Motor_ID, 30);

  delay(3000);

  R_Motor.goalSpeed(R_Motor_ID, -30);
  L_Motor.goalSpeed(L_Motor_ID, -30);

  delay(3000);
}
