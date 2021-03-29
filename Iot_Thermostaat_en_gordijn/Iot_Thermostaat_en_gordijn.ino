// include the library code:
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter(0x23);

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "K40rFyqcPMu6dGihL5QOiAeXBFKyqyke";

WidgetLED led1(V2);
WidgetLED led2(V7);
WidgetLED led3(V8);
WidgetLED led4(V10);

#define ONE_WIRE_BUS 26

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int RS = 2, EN = 4, d4 = 5, d5 = 18, d6 = 19, d7 = 23;

const int buttonPin1 = 27;     // the number of the pushbutton pin
const int buttonPin2 = 25;     // the number of the pushbutton pin

int buttonState1 = 0;
int buttonState2 = 0;
int led1Pin= 32;
int led2Pin= 33;
int Counter = 18;
int Temp = 0;
int stand = LOW;
int omlaag = LOW;
int omhoog = LOW;
int geopent = LOW;
int gesloten = HIGH;
int red_light_pin= 13;
int green_light_pin = 12;
int blue_light_pin = 14;



LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);
OneWire oneWire(ONE_WIRE_BUS);
float temp=0.0;
DallasTemperature sensors(&oneWire);

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "telenet-32924";
char pass[] = "YmutaKcebv8p";





void setup() {
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  

  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");  

  Blynk.begin("K40rFyqcPMu6dGihL5QOiAeXBFKyqyke", ssid, pass, "server.wyns.it", 8081);
  Wire.begin();
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  lcd.begin(16, 2);
  ;sensors.begin();
  lightMeter.begin();
  

}


void loop() {
  
 sensors.requestTemperatures(); 
  Serial.print("Temperature: ");
  Serial.println(sensors.getTempCByIndex(0)); 
  lcd.clear();
  lcd.print("Temp=");
  lcd.setCursor(0, 1);
  lcd.print("Set=");
  lcd.setCursor(6, 0);
  lcd.print(sensors.getTempCByIndex(0));
  lcd.setCursor(5, 1);
  lcd.print(Counter);
  Temp = (sensors.getTempCByIndex(0));
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
     Blynk.run();
  Blynk.virtualWrite(V1 , Temp);
  Blynk.virtualWrite(V4 , lux);
  

  buttonState1 = digitalRead(27);
 if (buttonState1 == 1){
   Counter = Counter + 1;
    Serial.println(Counter);
    lcd.setCursor(5, 1);
    lcd.print(Counter);
  }
  buttonState2 = digitalRead(25);
 if (buttonState2 == 1){
   Counter = Counter - 1;
    Serial.println(Counter);
    lcd.setCursor(5, 1);
    lcd.print(Counter);
 }

 if (Counter < Temp){
  RGB_color(0, 255, 0); // Green
  delay(1000);
  led1.off();
 }
 if (Counter > Temp){
  RGB_color(255, 0, 0); // Red
  delay(1000);
  led1.on();
 }

 if (lux < 50 && stand == LOW && omlaag == LOW){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Gordijn gaat");
  lcd.setCursor(0, 2);
  lcd.print("Omlaag");
  led3.on();
  led2.off();
  digitalWrite(led1Pin, HIGH);
  digitalWrite(led2Pin, LOW);
  omlaag = HIGH;
  omhoog = LOW;
  delay(1000 * 5);
 }
 
 if (lux > 50 && stand == LOW && omhoog ==LOW){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Gordijn gaat");
  lcd.setCursor(0, 2);
  lcd.print("Omhoog");
  led2.on();
  led3.off();
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, HIGH);
  omlaag = LOW;
  omhoog = HIGH;
  delay(1000 * 5);
 }
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
 digitalWrite(red_light_pin, red_light_value);
 digitalWrite(green_light_pin, green_light_value);
 digitalWrite(blue_light_pin, blue_light_value);
}

BLYNK_WRITE(V3)
{

int pinValue = param.asInt();

Serial.println("The value is:");

Serial.println(pinValue);

 if (pinValue < Temp){
  RGB_color(0, 255, 0); // Green
  delay(1000);
  led1.off();
  Counter = pinValue;
  lcd.setCursor(5, 1);
  lcd.print(Counter);
  
 }
 if (pinValue > Temp){
  RGB_color(255, 0, 0); // Red
  delay(1000);
  led1.on();
  Counter = pinValue;
  lcd.setCursor(5, 1);
  lcd.print(Counter);
  
 }
 
 
}

BLYNK_WRITE(V5)
{
int buttonState5 = param.asInt();


 if (buttonState5 == HIGH){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Gordijn gaat");
  lcd.setCursor(0, 2);
  lcd.print("Omhoog");
  led2.on();
  led3.off();
  omlaag = LOW;
  omhoog = HIGH;
  delay(1000 * 5);
  
 }

}
BLYNK_WRITE(V6)
{
int buttonState6 = param.asInt();


 if (buttonState6 == HIGH){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Gordijn gaat");
  lcd.setCursor(0, 2);
  lcd.print("Omlaag");
  led3.on();
  led2.off();
  omlaag = HIGH;
  omhoog = LOW;
  delay(1000 * 5);
  
 }

}


BLYNK_WRITE(V9)
{
int buttonState9 = param.asInt();

 if (buttonState9 == HIGH){
  stand = HIGH;
 }
 if (buttonState9 == LOW){
  stand = LOW;
}

}

BLYNK_WRITE(V11)
{
int buttonState11 = param.asInt();

 if (buttonState11 == HIGH && geopent == HIGH){
  led4.on();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Garage open");
  geopent = LOW;
  gesloten = HIGH;
  delay(1000 * 5);
 }
 if (buttonState11 == LOW && gesloten == HIGH){
  led4.off();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Garage dicht");
  geopent = HIGH;
  gesloten = LOW;
  delay(1000 * 5);
  
}

}
