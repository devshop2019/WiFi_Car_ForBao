#include "exten_function.h"

#include "Car_Config.h"

String _buzzer, _ledLeft, _ledRight, _controller, _speed, _pin, _servo;

Vietduino_Led       myBuzzer(PIN_BUZZER, ACTIVE_SIGNAL_BUZZER);
Vietduino_DCmotor   myMotor(PIN_DIR, PIN_PWM);
Vietduino_Servo     myServo(PIN_SERVO, SERVO_GO);

CREATE_FUNCTION(RunLed){
  while(1){
    myBuzzer.write(100,100,4);
    myMotor.forward(255);
    myServo.write(SERVO_GO_LEFT);
    M_DELAY(2000);
    myBuzzer.write(500,100,2);
    myMotor.backward(160);
    myServo.write(SERVO_GO_RIGHT);
    M_DELAY(4000);
  }
  
  END_CREATE_FUNCTION
}

CREATE_FUNCTION(ReadSerial){
  wifiSerial.begin(115200);
  DB_SERIAL_KXN("Start");

  while(1)
  {
    if (wifiSerial.available()) {
      String command = wifiSerial.readStringUntil('\n');
      DB_SERIAL_KXN(command);
      handlingdata(command);
  
    }
    M_DELAY(0);
  }
  
  END_CREATE_FUNCTION
}

//command=value,    Offset0 = command, offset1 = value
//Update--11/7/2019
#define DB_SPLIT(...)     Serial.println(__VA_ARGS__)
//#define DB_SPLIT(...)

String splitString(String v_G_motherString, String v_G_Command, String v_G_Start_symbol, String v_G_Stop_symbol, unsigned char v_G_Offset) {
    unsigned char tem_Offset = v_G_Offset - 1;
    unsigned char lenOfCommand=v_G_Command.length();
    
    int posOfCommand=v_G_motherString.indexOf(v_G_Command);  
    int PosOfStartSymbol=v_G_motherString.indexOf(v_G_Start_symbol,posOfCommand+lenOfCommand);
    if(posOfCommand < 0) return "";

    // Xu ly Dau Command
    DB_SPLIT(v_G_Command+ " posOfCommand " + (String)posOfCommand);
    if(posOfCommand > 0){
      String tem__ = v_G_motherString.substring(posOfCommand-1, posOfCommand);
      DB_SPLIT("posOfCommand at Start " + tem__);
      if((tem__ != " ") && (tem__ != v_G_Stop_symbol)){
        return "";
      }
    }

    // Xu ly cuoi Command
    unsigned int temPosOfLastCommand = posOfCommand + lenOfCommand;
    if(temPosOfLastCommand){
      String tem__ = v_G_motherString.substring(temPosOfLastCommand, temPosOfLastCommand + 1);
      DB_SPLIT("posOfCommand at Last " + tem__);
      if((tem__ != " ") && (tem__ != v_G_Start_symbol)){
        return "";
      }
    }

    if(v_G_Offset == 0){
        String tem__ = v_G_motherString.substring(posOfCommand, PosOfStartSymbol);
        if(tem__ == v_G_Command){
            return tem__;
        }
        return  "";
    }
  
    while(tem_Offset > 0){
        tem_Offset--;
        PosOfStartSymbol=v_G_motherString.indexOf(v_G_Start_symbol,PosOfStartSymbol+1);
    }
  
  if(v_G_Stop_symbol != ""){
    int PosOfStopSymbol=v_G_motherString.indexOf(v_G_Stop_symbol,PosOfStartSymbol+1);
    if(PosOfStopSymbol == -1){
      return "";
    }else;    
    return v_G_motherString.substring(PosOfStartSymbol+1,PosOfStopSymbol);
  }else{
    return v_G_motherString.substring(PosOfStartSymbol+1);
  }
}



void handlingdata(String data) {
  _speed = splitString(data, "Speed", "=", ",", 1);
  _ledRight = splitString(data, "LedRight", "=", ",", 1);
  _ledLeft =  splitString(data, "LedLeft", "=", ",", 1);
  _buzzer  =  splitString(data, "Buzzer", "=", ",", 1);
  _controller = splitString(data, "Controller", "=", ",", 0);
  DB_SERIAL_KXN("Controller: " + _controller);
  DB_SERIAL_KXN("Buzzer: " + _buzzer);
//  checkController(_controller,_speed,_servo);
//  checkBuzzer(_buzzer);
//  checkLight(_ledLeft,_ledRight);
}
