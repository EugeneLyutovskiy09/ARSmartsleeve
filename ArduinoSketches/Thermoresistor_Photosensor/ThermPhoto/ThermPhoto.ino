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
