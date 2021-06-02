/*
https://lastminuteengineers.com/creating-esp8266-web-server-arduino-ide/

*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/*Put your SSID & Password*/
const char* ssid = "Hshop.vn Guest";  // Enter SSID here
const char* password = "likehshop";  //Enter Password here

const char* ssid_ap = "Virus 2";  // Enter SSID here
const char* password_ap = "likehshop";  //Enter Password here

ESP8266WebServer server(80);

uint8_t LED1pin = 13;
bool LED1status = LOW;

uint8_t Buzzer_pin = 16;
bool LED2status = LOW;
#define _ACTIVE_ON_     HIGH
#define _ACTIVE_OFF_    !_ACTIVE_ON_
enum{
  _NOBODY_,
  _VIP_,
  _CUSTOMER_
};

enum{
  _TIME_BEEP_VIP_ = 100,
  _TIME_BEEP_CUSTOMER_ = 500,
  _COUNT_BEEP_VIP_ = 10,
  _COUNT_BEEP_CUSTOMER_ = 3
};
int i_Mode = _NOBODY_;

int i_CountBeep = 0;
int i_CountBeep_Set = _COUNT_BEEP_CUSTOMER_;
unsigned long ul_TimeBeep = _TIME_BEEP_CUSTOMER_;
unsigned long ul_TimeLastBeep = 0;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(LED1pin, OUTPUT);
  pinMode(Buzzer_pin, OUTPUT);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  int tempCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  tempCount++;
  if(tempCount >= 5) {
    WiFi.softAP(ssid_ap, password_ap);
    break;
  }
  Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect2);
  server.on("/led1on", handle_Customer);
  server.on("/led1off", handle_Customer);
  server.on("/led2on", handle_Vip);
  server.on("/led2off", handle_Vip);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  LoopBuzzer();
//  if(LED1status)
//  {digitalWrite(LED1pin, HIGH);}
//  else
//  {digitalWrite(LED1pin, LOW);}
//  
//  if(LED2status)
//  {digitalWrite(Buzzer_pin, HIGH);}
//  else
//  {digitalWrite(Buzzer_pin, LOW);}
}

void LoopBuzzer(){
  if(i_Mode != _NOBODY_){
    if(millis() - ul_TimeLastBeep >= ul_TimeBeep){
      ul_TimeLastBeep = millis();
      if((i_CountBeep%2) == 0){
        digitalWrite(Buzzer_pin, _ACTIVE_ON_);
        Serial.println("Buzzer on");
      }else{
        digitalWrite(Buzzer_pin, _ACTIVE_OFF_);
        Serial.println("Buzzer off");
      }
      i_CountBeep++;
      if((i_CountBeep/2) >= i_CountBeep_Set){
        digitalWrite(Buzzer_pin, _ACTIVE_OFF_);
        i_Mode = _NOBODY_;
        i_CountBeep = 0;
        Serial.println("Buzzer finish!");
      }
    }
  }
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  i_Mode = _NOBODY_;
  Serial.println("Client Connected!");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handle_OnConnect2() {
  LED1status = LOW;
  LED2status = LOW;
  i_Mode = _NOBODY_;
  Serial.println("Client Connected!");
  server.send(200, "text/html", SendHTML2(LED1status,LED2status)); 
}

void handle_Customer() {
  LED1status = LOW;
  LED2status = LOW;
  i_Mode = _CUSTOMER_;
  ul_TimeBeep = _TIME_BEEP_CUSTOMER_;
  i_CountBeep_Set = _COUNT_BEEP_CUSTOMER_;
  i_CountBeep = 0;
  digitalWrite(Buzzer_pin, _ACTIVE_ON_);
  Serial.println("Customer detect!");
  server.send(200, "text/html", SendHTML2(LED1status,LED2status)); 
}

void handle_Vip() {
  LED1status = LOW;
  LED2status = LOW;
  i_Mode = _VIP_;
  ul_TimeBeep = _TIME_BEEP_VIP_;
  i_CountBeep_Set = _COUNT_BEEP_VIP_;
  i_CountBeep = 0;
  digitalWrite(Buzzer_pin, _ACTIVE_ON_);
  Serial.println("Vip detect!");
  server.send(200, "text/html", SendHTML2(LED1status,LED2status)); 
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status)); 
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status)); 
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO6 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true)); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
    ptr +="<h3>Using Station(STA) Mode</h3>\n";
  
   if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

String SendHTML2(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 160px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
    ptr +="<h3>Using Station(STA) Mode</h3>\n";
  
   if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">CUSTOMER</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">CUSTOMER</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">VIP</a>\n";}
  else
  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">VIP</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
