#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <Servo.h>
Servo s;

#include<DHT.h>
DHT d(13, DHT11);

#include <Keypad.h>
const byte ros = 4;
const byte col = 4;
char keys[ros][col] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'C' },
};
byte ropi[ros] = {2,3,4,5};
byte copi[col] = {6,7,8,9};
Keypad kp = Keypad(makeKeymap(keys), ropi, copi, ros, col);

String password = "1234"; // password to unlock the door
String inputstring = "";  // String for typed-in password 

void setup() {
  d.begin();             // DHT Begin
  Serial.begin(9600);    // Serial Monitor
  lcd.begin();           // LCD

  lcd.setCursor(0, 0);
  lcd.print("Enter password: ");
  lcd.setCursor(0, 1);

  pinMode(11, INPUT);    // LDR
  pinMode(12, INPUT);    // IR
  s.attach(10, 500, 2500); // Servo
  pinMode(A0, OUTPUT);   // LED 2
  pinMode(A1, OUTPUT);   // DC Motor
  pinMode(A2, OUTPUT);   // Piezo buzzer
  pinMode(A3, OUTPUT);   // LED 1

  s.write(0);
  auto_Lights();
  delay(1000);
}
void loop() {
  contactlessdoorbell();
  security();
  autofan();
  auto_Lights();
}


void security()
{
  char key = kp.getKey();
  if (key) { // is key pressed?
    if (inputstring.length() <= 4) {
      inputstring = inputstring + key;
      lcd.print("#");
    }
  }

  if (inputstring.length() == 4) { if inputstring legth is equal to the password's length
    if (inputstring == password) {
      delay(500);
      lcd.clear();
      lcd.print("Correct passcode");
      s.write(90); // open door
      delay(3000);
      s.write(0); // close door
    }
    else
    {
      delay(500);
      lcd.clear();
      lcd.print("Wrong passcode");
      s.write(0);
    }
    inputstring = "";
    delay(3000);
    lcd.clear();
    lcd.print("Enter Password: ");
    lcd.setCursor(0, 1);
  }

}

void auto_Lights()
{
  int ldrValue = digitalRead(11);

  if (ldrValue == HIGH) { // no light?
    digitalWrite(A0, HIGH); //turn on light
    digitalWrite(A3, HIGH); //turn on light
  } 
  if (ldrValue == LOW) { // light detected?
    digitalWrite(A0, LOW); //turn off light
    digitalWrite(A3, LOW); //turn off light
  }
}
void autofan() {
  int hum = d.readHumidity();

  if (hum >= 77) { // is it too humid?
    analogWrite(A1, 255); // turn on fan
  }
  if (hum < 77) { // is it not humid
    analogWrite(A1, 0); // turn off fan
  }
}
void contactlessdoorbell() {

  int IRValue = digitalRead(12);
  if (IRValue == LOW){ // hand detected?
    analogWrite(A2, 255); // ring bell
    delay(500);
    analogWrite(A2, 0);
    delay(1000);
  }
  if (IRValue == HIGH) {
    analogWrite(A2, 0);
    delay(1000);
  }
}
