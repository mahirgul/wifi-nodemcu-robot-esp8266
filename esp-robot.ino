
#include <ESP8266WiFi.h>

#define SendKey 0  //Button to send data Flash BTN on NodeMCU

int port = 5006;  //Port number
WiFiServer server(port);

//Server connect to WiFi Network
const char *ssid = "YourSSID";  //Enter your wifi SSID
const char *password = "YourPass";  //Enter your wifi Password

int regPins[3] = {D6, D7, D8};
//74HC595 pins latchPin = D8; clockPin = D7; dataPin = D6;

int motor[4] = {D1, D2, D3, D4};
//L293D Motor control speed pins

int sped = 200;
//Startup motor speed

int contPins[2] = {D0, D5};
//other control pins light etc.

byte dataArray[4] = {39, 216, 149, 106};
//Motor control serial data 39 Forward ---- 216 Backward ---- 149 Left ---- 106 Right

String moveData = "";
String Data = "";

WiFiClient client;

void setup()
{
  Serial.begin(115200);

  pinMode(regPins[0], OUTPUT);
  pinMode(regPins[1], OUTPUT);
  pinMode(regPins[2], OUTPUT);

  pinMode(contPins[0], OUTPUT);
  pinMode(contPins[1], OUTPUT);

  pinMode(motor[1], OUTPUT);
  pinMode(motor[2], OUTPUT);
  pinMode(motor[3], OUTPUT);
  pinMode(motor[4], OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to wifi

  // Wait for connection
  Serial.println("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.print("Open Telnet and connect to IP:");
  Serial.print(WiFi.localIP());
  Serial.print(" on port ");
  Serial.println(port);
}
//=======================================================================
//                    Loop
//=======================================================================

void loop()
{
  if (moveData == "F")
    forward();
  else if (moveData == "B")
    backward();
  else if (moveData == "L")
    left();
  else if (moveData == "R")
    right();
  else if (moveData == "S")
    mstop();
  else if (moveData == "Q")
    controlOut(moveData);
  else if (moveData == "W")
    controlOut(moveData);
  else if (moveData == "q")
    controlOut(moveData);
  else if (moveData == "w")
    controlOut(moveData);

  if (client)
  {
    if (client.connected())
    {
      Data = client.read();
      Data.trim();
      if (Data == "F" || Data == "B" || Data == "L" || Data == "R" || Data == "S" || Data == "Q" || Data == "W" || Data == "q" || Data == "w")
      {
        moveData = Data;
        Serial.println(moveData);
      }
      else if (Data == "0" || Data == "1" || Data == "2" || Data == "3" || Data == "4" || Data == "5" || Data == "6" || Data == "7" || Data == "8" || Data == "9")
      {
        Serial.println(Data);
        setSped(Data);
      }
    }
    else
    {
      Serial.println("Client not Connected1");
      client = server.available();
      delay(1000);
    }
  }
  else
  {
    Serial.println("Client not Connected2");
    client = server.available();
    delay(1000);
  }
}

void controlOut(String _data)
{
  if (_data == "Q")
    digitalWrite(contPins[0], HIGH);
  else if (_data == "q")
    digitalWrite(contPins[0], LOW);
  else if (_data == "W")
    digitalWrite(contPins[1], HIGH);
  else if (_data == "w")
    digitalWrite(contPins[1], LOW);
}

void movement(int b)
{
  digitalWrite(regPins[2], HIGH);
  shiftOut(regPins[0], regPins[1], MSBFIRST, b);
  digitalWrite(regPins[2], LOW);
}

void setSped(String sp)
{
  int intData = sp.toInt();
  switch (intData) {
    case 0:
      sped = 120;
      break;
    case 1:
      sped = 150;
      break;
    case 2:
      sped = 180;
      break;
    case 3:
      sped = 190;
      break;
    case 4:
      sped = 200;
      break;
    case 5:
      sped = 220;
      break;
    case 6:
      sped = 230;
      break;
    case 7:
      sped = 240;
      break;
    case 8:
      sped = 250;
      break;
    case 9:
      sped = 254;
      break;
  }
}

void setSpeed(int sp, int mt)
{
  analogWrite(motor[mt], sp);
}

void forward()
{
  setSpeed(sped, 1);
  setSpeed(sped, 2);
  setSpeed(sped, 3);
  setSpeed(sped, 4);
  movement(dataArray[0]);
}

void backward()
{
  setSpeed(sped, 1);
  setSpeed(sped, 2);
  setSpeed(sped, 3);
  setSpeed(sped, 4);
  movement(dataArray[1]);
}

void left()
{
  setSpeed(sped, 1);
  setSpeed(sped, 2);
  setSpeed(sped, 3);
  setSpeed(sped, 4);
  movement(dataArray[2]);
}

void right()
{
  setSpeed(sped, 1);
  setSpeed(sped, 2);
  setSpeed(sped, 3);
  setSpeed(sped, 4);
  movement(dataArray[3]);
}

void mstop()
{
  setSpeed(0, 1);
  setSpeed(0, 2);
  setSpeed(0, 3);
  setSpeed(0, 4);
  movement(0);
}
