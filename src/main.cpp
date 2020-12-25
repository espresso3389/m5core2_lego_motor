#include <M5Core2.h>
#include <Wire.h>
#include "Free_Fonts.h" 

#define SLAVE_ADDR      0x56
#define MOTOR_ADDR_BASE      0x00
#define ENCODER_ADDR_BASE      0x08
#define STEP_V      51
#define FRONT 4
#define X_LOCAL  60
#define Y_LOCAL  80
#define XF  30
#define YF  30

int16_t Speed = 0;

// copied from Wire.h and Replace Wire with Wire1
bool writeBytes(uint8_t address, uint8_t subAddress, uint8_t *data, uint8_t length) {
  bool function_result = false;

  #ifdef I2C_DEBUG_TO_SERIAL
    Serial.printf("writeBytes:send to 0x%02x [0x%02x] data=",address,subAddress);
  #endif

  Wire1.beginTransmission(address);                // Initialize the Tx buffer
  Wire1.write(subAddress);                         // Put slave register address in Tx buffer
  for(int i = 0; i < length; i++) {
    Wire1.write(*(data+i));                        // Put data in Tx buffer
    #ifdef I2C_DEBUG_TO_SERIAL
      Serial.printf("%02x ", *(data+i));
    #endif
  }
  function_result = (Wire1.endTransmission() == 0);  // Send the Tx buffer

  #ifdef I2C_DEBUG_TO_SERIAL
    Serial.printf("result:%s\n", function_result ? "OK" : "NG");
  #endif

  return function_result;             // Send the Tx buffer
}

bool readBytes(uint8_t address, uint8_t subAddress, uint8_t count,uint8_t * dest) {
  #ifdef I2C_DEBUG_TO_SERIAL
    Serial.printf("readBytes:read from 0x%02x [0x%02x] requestByte=%d receive=", address, subAddress, count);
  #endif

  Wire1.beginTransmission(address);   // Initialize the Tx buffer
  Wire1.write(subAddress);            // Put slave register address in Tx buffer
  uint8_t i = 0;
  if (Wire1.endTransmission(false) == 0 && Wire1.requestFrom(address, (uint8_t)count)) {
    while (Wire1.available()) {
      dest[i++] = Wire1.read();// Put read results in the Rx buffer
      #ifdef I2C_DEBUG_TO_SERIAL
        Serial.printf("%02x ", dest[i-1]);
      #endif
    }
    #ifdef I2C_DEBUG_TO_SERIAL
      Serial.printf(" (len:%d)\n", i);
    #endif
    return true;
  }
  #ifdef I2C_DEBUG_TO_SERIAL
    Serial.printf("none\n");
  #endif
  return false;
}


/*************************************************
Function:MotorRun
Description: Motor forward and reverse API
Input:
      n:Motor 0 to motor 3
  Speed:Speed value from -255 to +255,when speed=0,The motor stopped. 
  
Return: Successful return 1
Others: 
*************************************************/
int32_t MotorRun(uint8_t n,int16_t Speed){
    if(n>3)  
         return 0;
  
    if(Speed <= -255)
        Speed = -255;
  
    if(Speed >=255)
        Speed = 255;
       
    writeBytes(SLAVE_ADDR,MOTOR_ADDR_BASE+n*2,(uint8_t *)&Speed,2);
    
    return 1;
}

/*************************************************
Function:ReadEncoder
Description: Motor rotation code value
Input:
      n:Motor 0 to motor 3 
  
Return: Successful return 1
Others: 
*************************************************/
int32_t ReadEncoder(uint8_t n){
    uint8_t  dest[4]={0};

    if(n>3)  
        return 0;
    
    readBytes(SLAVE_ADDR,ENCODER_ADDR_BASE+n*4,4,dest);
    
    return *((int32_t*)dest);
}


/*************************************************
Function:header
Description:The UI title
Input:
Return: 
Others: 
*************************************************/
void header(const char *string, uint16_t color){
    M5.Lcd.fillScreen(color);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_MAGENTA, TFT_BLUE);
    M5.Lcd.fillRect(0, 0, 320, 30, TFT_BLUE);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString(string, 160, 3, 4);
   
}

/*************************************************
Function:motor_demo
Description:Press the button to control motor rotation,
            and coded values are displayed in real time.
Input:
Return: 
Others:A -> Speed--   B -> Speed=0   C -> Speed++  
*************************************************/
void motor_demo(void){
  uint8_t BtnFlag = 0;
  M5.update();
  
  if (M5.BtnA.wasReleased()) {
    Speed -= STEP_V;

    if(Speed <= -255)
    Speed = -255;

    BtnFlag = 1;
  }else if (M5.BtnB.wasReleased()) {
    Speed = 0;
    BtnFlag = 1;
  }
  else if (M5.BtnC.wasReleased()) {
    Speed += STEP_V;

    if(Speed >=255)
    Speed = 255;

    BtnFlag = 1;
  }  

  if(BtnFlag == 1){
      BtnFlag = 0;
      for(int i =0;i<4;i++){
          MotorRun(i,Speed);    
          M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + YF*i , FRONT);
          M5.Lcd.printf("S%d:%d       \n",i,Speed);
      }
  }

  for(int i=0;i<4;i++){
    M5.Lcd.setCursor(X_LOCAL + XF*4, Y_LOCAL + YF*i , FRONT);
    M5.Lcd.printf("E%d:%d           \n",i,ReadEncoder(i));
  }
}

void setup() {
    M5.begin();
    //M5.Power.begin();
    Serial.begin(115200);
    M5.Lcd.fillScreen(TFT_BLACK);               
    header("L E G O +", TFT_BLACK);
    M5.Lcd.setTextColor(TFT_WHITE,TFT_BLACK);
    for(int i =0;i<4;i++){
      M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + YF*i , FRONT);
      M5.Lcd.printf("S%d:%d     \n",i,Speed);
   }   
}

void loop() {
    motor_demo();   
}