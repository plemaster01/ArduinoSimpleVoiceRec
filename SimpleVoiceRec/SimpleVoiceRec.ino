/*!
 * @file uart.ino
 * @brief   Control the voice recognition module via UART
 * @n  Get the recognized command ID and play the corresponding reply audio according to the ID;
 * @n  Set the wake-up state duration, set mute mode, set volume, enter the wake-up state, and reset the module
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-04-02
 * @url  https://github.com/DFRobot/DFRobot_DF2301Q
 */
#include "DFRobot_DF2301Q.h"


#define Led1 2
#define Led2 3
#define Led3 4

/**
   @brief DFRobot_URM13_RTU constructor
   @param serial - serial ports for communication, supporting hard and soft serial ports
   @param rx - UART The pin for receiving data
   @param tx - UART The pin for transmitting data
*/
#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))  // Use software serial
SoftwareSerial softSerial(/*rx =*/12, /*tx =*/11);
DFRobot_DF2301Q_UART asr(/*softSerial =*/&softSerial);
#elif defined(ESP32)  // Use the hardware serial with remappable pin: Serial1
DFRobot_DF2301Q_UART asr(/*hardSerial =*/&Serial1, /*rx =*/11, /*tx =*/12);
#else                 // Use hardware serial: Serial1
DFRobot_DF2301Q_UART asr(/*hardSerial =*/&Serial1);

#endif

void setup() {
  Serial.begin(115200);

  pinMode(Led1, OUTPUT);    
  digitalWrite(Led1, LOW);  
  pinMode(Led2, OUTPUT);    
  digitalWrite(Led2, LOW);
  pinMode(Led3, OUTPUT);    
  digitalWrite(Led3, LOW);  

  // Init the sensor
  while (!(asr.begin())) {
    //asr.begin();
    delay(300);
  }
  
  //Serial.println("Begin ok!");

  /**
     @brief Reset module 
  */
  // asr.resetModule();

  /**
     @brief Set commands of the module
     @param setType - Set type
     @n       DF2301Q_UART_MSG_CMD_SET_VOLUME: Set volume, the set value range 1-7 
     @n       DF2301Q_UART_MSG_CMD_SET_ENTERWAKEUP: Enter wake-up state; set value 0
     @n       DF2301Q_UART_MSG_CMD_SET_MUTE Mute mode; set value 1: mute, 0: unmute
     @n       DF2301Q_UART_MSG_CMD_SET_WAKE_TIME ; Wake-up duration; the set value range 0-255s
     @param setValue - Set value, refer to the set type above for the range
  */
  asr.settingCMD(DF2301Q_UART_MSG_CMD_SET_MUTE, 0);
  asr.settingCMD(DF2301Q_UART_MSG_CMD_SET_VOLUME, 8);
  asr.settingCMD(DF2301Q_UART_MSG_CMD_SET_WAKE_TIME, 20);
  //asr.settingCMD(DF2301Q_UART_MSG_CMD_SET_ENTERWAKEUP, 0);

  /**
     @brief Play the corresponding reply audio according to the command word ID
     @param CMDID - Command word ID
  */
  asr.playByCMDID(23);
}

void loop() {
  /**
     @brief  Get the ID corresponding to the command word 
     @return Return the obtained command word ID, returning 0 means no valid ID is obtained
  */
  uint8_t CMDID = asr.getCMDID();
  switch (CMDID) {
    case 103:                                                  //If the command is “Turn on the light”
      digitalWrite(Led1, HIGH);                                 //Turn on the LED
      digitalWrite(Led2, HIGH);
      digitalWrite(Led3, HIGH);                                 //Turn on the LED
      break;

    case 104:                                                  //If the command is “Turn off the light”
      digitalWrite(Led1, LOW);                                 //Turn on the LED
      digitalWrite(Led2, LOW);
      digitalWrite(Led3, LOW);                                 //Turn on the LED
      break;
    
    case 5: 
      digitalWrite(Led1, LOW);                                 
      digitalWrite(Led2, LOW);
      digitalWrite(Led3, LOW);                                   
      delay(250);                                             //If the command is “All On”
      digitalWrite(Led1, HIGH);  
      delay(250);                               
      digitalWrite(Led2, HIGH);
      delay(250);
      digitalWrite(Led3, HIGH);
      delay(250);                                                           
      break;

    case 6:                                                  //If the command is “all off”
      digitalWrite(Led1, HIGH);                                 
      digitalWrite(Led2, HIGH);
      digitalWrite(Led3, HIGH);                                    
      delay(250);                                             
      digitalWrite(Led1, LOW);  
      delay(250);                               
      digitalWrite(Led2, LOW);
      delay(250);
      digitalWrite(Led3, LOW);
      delay(250);                                
      break;
    
    case 7:                                                  //If the command is “light one ON (blue)”
      digitalWrite(Led1, HIGH);
      break;
    
    case 8:
      digitalWrite(Led1, LOW); 
      break;                             //If the command is “light one OFF (blue)”
    
    case 9:                                                  //If the command is “light two ON (red)”
      digitalWrite(Led2, HIGH);
      break;
    
    case 11:
      digitalWrite(Led2, LOW);  
      break;                            //If the command is “light two OFF (red)”

    case 10:                                                  //If the command is “light three ON (green)”
      digitalWrite(Led3, HIGH);
      break;

    
    case 12:
      digitalWrite(Led3, LOW);
      break;                              //If the command is “light three OFF (green)”
    
    case 13:                                           //If the command is “light cycle”       
      for (int i = 0; i < 10; i++) {
        int delay_time = 25 * i;
        digitalWrite(Led1, LOW);                                 
        digitalWrite(Led2, LOW);
        digitalWrite(Led3, LOW);                                   
        delay(delay_time);                                             
        digitalWrite(Led1, HIGH);  
        delay(delay_time);                               
        digitalWrite(Led2, HIGH);
        delay(delay_time);
        digitalWrite(Led3, HIGH);
        delay(delay_time);                                
      }              
      break;            
    
    default:
      if (CMDID != 0) {
        Serial.print("CMDID = ");  //Print command ID
        Serial.println(CMDID);
      }
  }
  delay(300);
}