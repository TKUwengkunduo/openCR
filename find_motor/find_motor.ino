/*
 * https://emanual.robotis.com/docs/en/software/arduino_ide/
 */


#include <DynamixelWorkbench.h>

#define DXL_BUS_SERIAL1 "1"            //Dynamixel on Serial1(USART1)  <-OpenCM9.04
#define DXL_BUS_SERIAL2 "2"            //Dynamixel on Serial2(USART2)  <-LN101,BT210
#define DXL_BUS_SERIAL3 "3"            //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#define DXL_BUS_SERIAL4 "/dev/ttyUSB0" //Dynamixel on Serial3(USART3)  <-OpenCR

#define BAUDRATE_NUM 3

DynamixelWorkbench dxl_wb;

void setup()
{
  Serial.begin(57600);
  while(!Serial); // Open a Serial Monitor

  uint8_t scanned_id[16] = {0, };
  uint8_t dxl_cnt = 0;
  uint32_t baud[BAUDRATE_NUM] = {9600, 57600, 1000000};
  uint8_t index = 0;
  uint8_t range = 100;

  while (index < BAUDRATE_NUM)
  {
    Serial.println(String(baud[index]) + " bps");

    dxl_wb.begin(DXL_BUS_SERIAL3, baud[index]);
    dxl_wb.scan(&scanned_id[0], &dxl_cnt, range);

    for (int i = 0; i < dxl_cnt; i++)
    {
      Serial.println("   id : " + String(scanned_id[i]) + "   Model Name : " + String(dxl_wb.getModelName(scanned_id[i])));
    }

    index++;    
  }
  Serial.println("End");
}

void loop()
{

}
