
#include <ESP8266WiFi.h>
#define SendKey 0  //Button to send data Flash BTN on NodeMCU


#define STASSID "GulHome"
#define STAPSK  "demet@mahir@123"

#define BAUD_SERIAL 115200
#define RXBUFFERSIZE 1024

#define STACK_PROTECTOR  512 // bytes

//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 64
const char* ssid = STASSID;
const char* password = STAPSK;

const int port = 5006;

WiFiServer server(port);
WiFiClient serverClients[MAX_SRV_CLIENTS];

int regPins[3] = {D6, D7, D8};
//74HC595 pins latchPin = D8; clockPin = D7; dataPin = D6;

int motor[4] = {D1, D2, D3, D4};
//L293D Motor control speed pins

//Startup motor speed
int startSpeed = 160;

int speed = 160;

int maxSpeed = 100;

int lastSpeed = 0;

int contPins[2] = {D0, D5};
//other control pins light etc.

byte dataArray[4] = {39, 216, 149, 106};
//Motor control serial data 39 Forward ---- 216 Backward ---- 149 Left ---- 106 Right

String moveData = "";
String lastMoveData = "";
String Data = "";

void setup() {

  Serial.begin(BAUD_SERIAL);
  Serial.setRxBufferSize(RXBUFFERSIZE);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connecting to Wifi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  //start server
  server.begin();

  pinMode(regPins[0], OUTPUT);
  pinMode(regPins[1], OUTPUT);
  pinMode(regPins[2], OUTPUT);

  pinMode(contPins[0], OUTPUT);
  pinMode(contPins[1], OUTPUT);

  pinMode(motor[1], OUTPUT);
  pinMode(motor[2], OUTPUT);
  pinMode(motor[3], OUTPUT);
  pinMode(motor[4], OUTPUT);

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Open Telnet and connect to IP:");
  Serial.print(WiFi.localIP());
  Serial.print(" on port ");
  Serial.println(port);

  moveData = "S";
  speed = 250;
}

void loop() {
  moveDataProcess();
  setSpeed();

  //check if there are any new clients
  if (server.hasClient()) {
    //find free/disconnected spot
    int i;
    for (i = 0; i < MAX_SRV_CLIENTS; i++)
      if (!serverClients[i]) { // equivalent to !serverClients[i].connected()
        serverClients[i] = server.available();
        break;
      }

    if (i == MAX_SRV_CLIENTS) {
      server.available().println("busy");

    }
  }

  for (int i = 0; i < MAX_SRV_CLIENTS; i++)
    while (serverClients[i].available() && Serial.availableForWrite() > 0) {
      // working char by char is not very efficient
      Data = serverClients[i].read();
      dataProcess(Data);
    }


  // determine maximum output size "fair TCP use"
  // client.availableForWrite() returns 0 when !client.connected()
  int maxToTcp = 0;
  for (int i = 0; i < MAX_SRV_CLIENTS; i++)
    if (serverClients[i]) {
      int afw = serverClients[i].availableForWrite();
      if (afw) {
        if (!maxToTcp) {
          maxToTcp = afw;
        } else {
          maxToTcp = std::min(maxToTcp, afw);
        }
      } else {
        // warn but ignore congested clients
      }
    }

  //check UART for data
  size_t len = std::min(Serial.available(), maxToTcp);
  len = std::min(len, (size_t)STACK_PROTECTOR);
  if (len) {
    uint8_t sbuf[len];
    int serial_got = Serial.readBytes(sbuf, len);
    // push UART data to all connected telnet clients
    for (int i = 0; i < MAX_SRV_CLIENTS; i++)
      // if client.availableForWrite() was 0 (congested)
      // and increased since then,
      // ensure write space is sufficient:
      if (serverClients[i].availableForWrite() >= serial_got) {
        size_t tcp_sent = serverClients[i].write(sbuf, serial_got);

      }
  }
}

void dataProcess(String Data)
{
  Data.trim();

  Serial.println(Data);

  if (Data == "F" || Data == "B" || Data == "L" || Data == "R" || Data == "S" || Data == "Q" || Data == "W" || Data == "q" || Data == "w")
  {
    moveData = Data;
  }
  else if (Data == "0" || Data == "1" || Data == "2" || Data == "3" || Data == "4" || Data == "5" || Data == "6" || Data == "7" || Data == "8" || Data == "9")
  {
    int intData = Data.toInt();
    speed = (intData * 10) + startSpeed;
    Serial.println(Data);
  }
  else if (Data == ".")
  {
    //Serial.println(Data);
  }
}

void moveDataProcess()
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

void setSpeed()
{
  if (maxSpeed < speed)
  {
    if (moveData != "S")
    {
      maxSpeed++;
      Serial.println(maxSpeed);
      analogWrite(motor[1], maxSpeed);
      analogWrite(motor[2], maxSpeed);
      analogWrite(motor[3], maxSpeed);
      analogWrite(motor[4], maxSpeed);
    }
  }
}

void setSpeed(int sp)
{
  maxSpeed = 100;
  analogWrite(motor[1], sp);
  analogWrite(motor[2], sp);
  analogWrite(motor[3], sp);
  analogWrite(motor[4], sp);
}

void forward()
{
  movement(dataArray[0]);
}

void backward()
{
  movement(dataArray[1]);
}

void left()
{
  movement(dataArray[2]);
}

void right()
{
  movement(dataArray[3]);
}

void mstop()
{
  movement(0);
  setSpeed(0);
}
