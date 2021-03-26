#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int switchPin = 8;
int hits = 0;
int countQuotes = 0;
int switchState = 0;
int prevSwitchState = 0;
String quotes[14] = {"Love for all", "hatred for none.", "Whatever you do,", "do it well.", "What we think,", "we become.", "Strive for", "greatness.", "And still,", "I rise.", "It hurt because", "it mattered.", "When words fail,", "music speaks."};
int tempPin = 1;
int temValue;
float tempCelsius;

void setup()
{
    Serial.begin(9600);
    lcd.clear();
    lcd.begin(16, 2);
    pinMode(switchPin, INPUT);
}

void loop()
{
    switchState = digitalRead(switchPin);
    if (switchState != prevSwitchState)
    {
        if (switchState == LOW)
        {
            hits++;
            switch (hits % 3)
            {
            case 0:
                timeDisplay();
                delay(300);
                break;
            case 1:
                weatherDisplay();
                delay(300);
                break;
            case 2:
                quotesDisplay();
                delay(300);
                break;
            }
        }
    }

    if (hits % 3 == 0)
    {
        timeDisplay();
        delay(200);
    }

    prevSwitchState = switchState;
}

void timeDisplay()
{
    lcd.clear();
    tmElements_t tm;
    if (RTC.read(tm))
    {
        lcd.setCursor(4, 0);
        print2digits(tm.Hour);
        lcd.write(':');
        print2digits(tm.Minute);
        lcd.write(':');
        print2digits(tm.Second);

        lcd.setCursor(3, 1);
        print2digits(tm.Day);
        lcd.write('/');
        print2digits(tm.Month);
        lcd.write('/');
        lcd.print(tmYearToCalendar(tm.Year));
    }
}

void print2digits(int number)
{
    if (number >= 0 && number < 10)
    {
        lcd.write('0');
    }
    lcd.print(number);
}

void weatherDisplay()
{
    lcd.clear();
    lcd.setCursor(2, 0);

    temValue = analogRead(tempPin);
    tempCelsius = temValue * 500 / 1023;

    lcd.print("Temperature: ");
    lcd.setCursor(6, 1);
    lcd.print(round(tempCelsius));
    lcd.write(0xDF);
    lcd.print("C");
}

void quotesDisplay()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    String text1 = quotes[countQuotes++];
    lcd.print(text1);
    lcd.setCursor(0, 1);
    String text2 = quotes[countQuotes++];
    lcd.print(text2);
    if (countQuotes > 13)
    {
        countQuotes %= 14;
    }
}
