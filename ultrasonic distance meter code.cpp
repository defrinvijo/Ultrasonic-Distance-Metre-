#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 9
#define ECHO_PIN 10

#define GREEN_LED 2
#define YELLOW_LED 3
#define RED_LED 4
#define BUZZER 5

LiquidCrystal_I2C lcd(0x27, 16, 2);

long duration;
float distance_cm;
float distance_inch;

void setup()
{
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("ULTRASONIC");
  lcd.setCursor(0,1);
  lcd.print("DISTANCE METER");
  delay(3000);
  lcd.clear();
}

void loop()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  distance_cm = duration * 0.0343 / 2;
  distance_inch = distance_cm / 2.54;

  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.print(" cm  |  ");
  Serial.print(distance_inch);
  Serial.println(" inch");

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dist:");
  lcd.print(distance_cm,1);
  lcd.print("cm");

  lcd.setCursor(0,1);
  lcd.print(distance_inch,1);
  lcd.print(" inch");

  // SAFE ZONE
  if(distance_cm > 50)
  {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER);

    lcd.setCursor(11,0);
    lcd.print("OK");
  }

  // WARNING ZONE
  else if(distance_cm > 20 && distance_cm <= 50)
  {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);

    tone(BUZZER, 1000);
    delay(100);
    noTone(BUZZER);

    lcd.setCursor(8,0);
    lcd.print("WARN");
  }

  // DANGER ZONE
  else
  {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);

    tone(BUZZER, 2000);

    lcd.setCursor(8,0);
    lcd.print("DANGER");
  }

  delay(300);
}