# AR with Arduino

Initially the idea was to create a smartsleeve with temperature and photo sensors. As I really wanted to build circuits and create AR at the same time. However It turned out to be way more difficult and time-consuming than I expected, therefore I decided to concentrate on the AR and sensor readings putting them together to have some fun with the help on Unity. 

## Getting Started

This is a brief overview of the project (its current progress). Here you will get familiarized with the main hardware and software (also coding) features of my project.

### Prerequisites of the project

As the project is based on a microcontroller and a bunch of sensors here are the main parts:

1.Arduino (in my case it is Uno)
2. T sensor (thermistors) - we need them just to track changes in temperature, no accuracy is needed 
3. Photosensor. Same here, no accuracy, just tracking the changes of intensity. 
4. Jump wires and a breadboard. Goes without saying that Arduino is useless without this things.
5. Wifi module (ESP8266 in my case): I intend to set an IoT for communication via WiFi. The readings will be transfered onto the cloud and to the Unity.
6. Software: Arduino IDE with ESP8266 board package, Unity (32-bits) with Uduino package for communication with Arduino, Vuforia for AR, Blender for 3d models. 

## tests and current progress

At this point I set up the circuit. Sensors are programmed and tested:

```
#define THERMISOR_PIN 0
uint16_t Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
const int pResistor = A3; // Photoresistor at Arduino analog pin A3
const int ledPin=4;  
int value;   
void setup() {
Serial.begin(9600);
pinMode(ledPin, OUTPUT);

}

void loop() {
Vo = analogRead(THERMISOR_PIN);
int sensorVal = analogRead(A0);
R2 = R1 * (1023.0 / (float)Vo - 1.0);
logR2 = log(R2);
T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
Tc = T - 273.15+50; //Temperature in Celsius (50 degrees is an inacuracy of the thermoresistor)
  value = analogRead(pResistor);
  value = map(value, 0,1023,0,255);
  if (value > 75){
    digitalWrite(ledPin, LOW);  //Turn led off
  }
  else{
    digitalWrite(ledPin, HIGH); //Turn led on
  }
Serial.println(value);
Serial.print("Temperature: ");
Serial.print(Tc);
Serial.print("C ");
delay(100);
}
```
Following step is to get the WiFi module (esp 8266) working. It turned out to be a real pain in the neck to get the thing do its work. So here is the code to connect it to the nearest wifi network:
```
#include <SoftwareSerial.h>

SoftwareSerial ESPserial(2, 3); // RX | TX

void setup()

{

Serial.begin(115200); // communication with the host computer

//while (!Serial) { ; }

// Start the software serial for communication with the ESP8266

ESPserial.begin(115200);

Serial.println("");

Serial.println("NL & CR in the serial monitor.");

Serial.println("Ready");

Serial.println("");

}

void loop()

{

// listen for communication from the ESP8266 and then write it to the serial monitor

if ( ESPserial.available() ) { Serial.write( ESPserial.read() ); }

// listen for user input and send it to the ESP8266

if ( Serial.available() ) { ESPserial.write( Serial.read() ); }

}
```
Next I needed to transfer the readings from the sensors to Unity. Initially I was going to make it with the help of esp8266 by means of IoT, however I am still having trouble with it. So to test the interaction as a prototype I decided to use Uduino asset from Unity. This allows you to transfer the readings form the microcontroller to Unity using the COM port. There is an extention allowing you to connect via Wifi, but unluckily it does not work with my device (anyhow it is supposed to be). The following code goes together with the asset from the Unity store, the link will be provided in the Acknowledgement and references box.  
```
// Uduino Default Board
#include<Uduino.h>
Uduino uduino("uduinoBoard"); //

// Servo
#include <Servo.h>
#define MAXSERVOS 8


void setup()
{
  Serial.begin(9600);

#if defined (__AVR_ATmega32U4__) // 
  while (!Serial) {}
#elif defined(__PIC32MX__)
  delay(1000);
#endif

  uduino.addCommand("s", SetMode);
  uduino.addCommand("d", WritePinDigital);
  uduino.addCommand("a", WritePinAnalog);
  uduino.addCommand("rd", ReadDigitalPin);
  uduino.addCommand("r", ReadAnalogPin);
  uduino.addCommand("br", BundleReadPin);
  uduino.addCommand("b", ReadBundle);
  uduino.addInitFunction(DisconnectAllServos);
  uduino.addDisconnectedFunction(DisconnectAllServos);
}

void ReadBundle() {
  char *arg = NULL;
  char *number = NULL;
  number = uduino.next();
  int len = 0;
  if (number != NULL)
    len = atoi(number);
  for (int i = 0; i < len; i++) {
    uduino.launchCommand(arg);
  }
}

void SetMode() {
  int pinToMap = 100; //100 is never reached
  char *arg = NULL;
  arg = uduino.next();
  if (arg != NULL)
  {
    pinToMap = atoi(arg);
  }
  int type;
  arg = uduino.next();
  if (arg != NULL)
  {
    type = atoi(arg);
    PinSetMode(pinToMap, type);
  }
}

void PinSetMode(int pin, int type) {
  //TODO : vérifier que ça, ça fonctionne
  if (type != 4)
    DisconnectServo(pin);

  switch (type) {
    case 0: // Output
      pinMode(pin, OUTPUT);
      break;
    case 1: // PWM
      pinMode(pin, OUTPUT);
      break;
    case 2: // Analog
      pinMode(pin, INPUT);
      break;
    case 3: // Input_Pullup
      pinMode(pin, INPUT_PULLUP);
      break;
    case 4: // Servo
      SetupServo(pin);
      break;
  }
}

void WritePinAnalog() {
  int pinToMap = 100;
  char *arg = NULL;
  arg = uduino.next();
  if (arg != NULL)
  {
    pinToMap = atoi(arg);
  }

  int valueToWrite;
  arg = uduino.next();
  if (arg != NULL)
  {
    valueToWrite = atoi(arg);

    if (ServoConnectedPin(pinToMap)) {
      UpdateServo(pinToMap, valueToWrite);
    } else {
      analogWrite(pinToMap, valueToWrite);
    }
  }
}

void WritePinDigital() {
  int pinToMap = -1;
  char *arg = NULL;
  arg = uduino.next();
  if (arg != NULL)
    pinToMap = atoi(arg);

  int writeValue;
  arg = uduino.next();
  if (arg != NULL && pinToMap != -1)
  {
    writeValue = atoi(arg);
    digitalWrite(pinToMap, writeValue);
  }
}

void ReadAnalogPin() {
  int pinToRead = -1;
  char *arg = NULL;
  arg = uduino.next();
  if (arg != NULL)
  {
    pinToRead = atoi(arg);
    if (pinToRead != -1)
      printValue(pinToRead, analogRead(pinToRead));
  }
}

void ReadDigitalPin() {
  int pinToRead = -1;
  char *arg = NULL;
  arg = uduino.next();
  if (arg != NULL)
  {
    pinToRead = atoi(arg);
  }
  
  if (pinToRead != -1)
    printValue(pinToRead, digitalRead(pinToRead));
}

void BundleReadPin() {
  int pinToRead = -1;
  char *arg = NULL;
  arg = uduino.next();
  if (arg != NULL)
  {
    pinToRead = atoi(arg);
    if (pinToRead != -1)
      printValue(pinToRead, analogRead(pinToRead));
  }
}

Servo myservo;
void loop()
{
  uduino.update();
}

void printValue(int pin, int targetValue) {
  Serial.print(pin);
  Serial.print(" "); //<- Todo : Change that with Uduino delimiter
  Serial.println(targetValue);
  // TODO : Here we could put bundle read multiple pins if(Bundle) { ... add delimiter // } ...
}




/* SERVO CODE */
Servo servos[MAXSERVOS];
int servoPinMap[MAXSERVOS];
/*
void InitializeServos() {
  for (int i = 0; i < MAXSERVOS - 1; i++ ) {
    servoPinMap[i] = -1;
    servos[i].detach();
  }
}
*/
void SetupServo(int pin) {
  if (ServoConnectedPin(pin))
    return;

  int nextIndex = GetAvailableIndexByPin(-1);
  if (nextIndex == -1)
    nextIndex = 0;
  servos[nextIndex].attach(pin);
  servoPinMap[nextIndex] = pin;
}


void DisconnectServo(int pin) {
  servos[GetAvailableIndexByPin(pin)].detach();
  servoPinMap[GetAvailableIndexByPin(pin)] = 0;
}

bool ServoConnectedPin(int pin) {
  if (GetAvailableIndexByPin(pin) == -1) return false;
  else return true;
}

int GetAvailableIndexByPin(int pin) {
  for (int i = 0; i < MAXSERVOS - 1; i++ ) {
    if (servoPinMap[i] == pin) {
      return i;
    } else if(pin == -1 && servoPinMap[i] < 0) {
     return i; // return the first available index 
    }
  }
  return -1;
}

void UpdateServo(int pin, int targetValue) {
  int index = GetAvailableIndexByPin(pin);
  servos[index].write(targetValue);
  delay(10);
}

void DisconnectAllServos() {
  for (int i = 0; i < MAXSERVOS; i++) {
    servos[i].detach();
    digitalWrite(servoPinMap[i], LOW);
    servoPinMap[i] = -1;
  }
}
```
## Acknowledgments and references

Uduino http://www.marcteyssier.com/uduino/


* Hat tip to anyone whose code was used
* Inspiration
* etc
