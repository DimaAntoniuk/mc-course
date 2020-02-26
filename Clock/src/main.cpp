#include <Arduino.h>

#include <Keypad.h>
#include <LiquidCrystal.h>

#define RS 7
#define ENABLE 6
#define D4 A3
#define D5 A2
#define D6 A1
#define D7 A0
#define ROWS 4
#define COLUMNS 4
#define BUZZER A12
#define BEEP_SHORT_DELAY 50
#define BEEP_LONG_DELAY 150

LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);

char hexaKeys[ROWS][COLUMNS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'#', '0', '*', 'D'}};
byte rowPins[ROWS] = {45, 44, 43, 42};
byte columnPins[COLUMNS] = {46, 47, 48, 49};

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, columnPins, ROWS, COLUMNS);

struct Time
{
  uint8_t second, minute, hour;
};
Time time = {0, 0, 0};
Time alarm;

ISR(TIMER1_COMPA_vect)
{
  if (++time.second == 60)
  {
    time.second = 0;
    if (++time.minute == 60)
    {
      time.minute = 0;
      if (++time.hour == 24)
        time.hour = 0;
    }
  }
}

inline void showTime(const Time &time);
inline void onPressA();
inline void onPressB();
inline void onPressC();
inline void onPressD();
bool apply();
inline bool checkTimeFormat(const Time &time);
inline void shortBeep();
inline void doubleShortBeep();
inline void longBeep();

void setup()
{
  noInterrupts();

  pinMode(BUZZER, OUTPUT);
  lcd.begin(16, 2);

  TCCR1A = WGM10;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); //CTC mode & Prescaler @ 1024
  TIMSK1 = (1 << OCIE1A);                            // дозвіл на переривання по співпадінню
  OCR1A = 0x3D08;                                    // 1 sec (16MHz AVR)

  interrupts();
}

void loop()
{
  char selectedKey = keypad.getKey();
  delay(50);
  switch (selectedKey)
  {
  case 'A':
    shortBeep();
    onPressA();
    break;

  case 'B':
    shortBeep();
    onPressB();
    break;

  case 'C':
    shortBeep();
    onPressC();
    break;

  case 'D':
    shortBeep();
    onPressD();
    break;

  default:
    showTime(time);
    delay(100);
    break;
  }
}

inline void onPressA()
{ 
  Time enteredTime;
  int numbers[6];
  int currentSize = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NewTime:");
  char enteredKey;
  while (currentSize < 6) {
    if (enteredKey = keypad.getKey()) {
      if (int(enteredKey) <= int('9') && int(enteredKey) >= int('0')) {
        lcd.setCursor(currentSize + currentSize / 2, 1);
        lcd.print(enteredKey);
        numbers[currentSize] = enteredKey - '0';
        currentSize ++;
        if (!(currentSize % 2) && currentSize < 5 && currentSize > 0) {
        lcd.print(":");
      }
      } else {
        lcd.setCursor(11, 0);
        lcd.print("Error");
        delay(500);
        lcd.setCursor(11, 0);
        lcd.print("     ");
      }
    }
  }

  enteredTime.hour = numbers[0] * 10 + numbers[1];
  enteredTime.minute = numbers[2] * 10 + numbers[3];
  enteredTime.second = numbers[4] * 10 + numbers[5];

  if (checkTimeFormat(enteredTime)) {
    if (apply()) {
      time = enteredTime;
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wrong time");
    lcd.setCursor(0, 1);
    lcd.print("Again?");
    if (apply()) {
      onPressA();
    }
  }
  showTime(time);
}

inline void onPressB()
{
}

inline void onPressC()
{
}

inline void onPressD()
{
}

bool apply()
{
  lcd.setCursor(9, 1);
  lcd.print("Y-# N-*");
  
  char currentKey;
  while(true){
    if(currentKey = keypad.getKey()) {
      switch(currentKey) {
          case '*':
            longBeep();
            return false;
          case '#':
            doubleShortBeep();
            return true;
          default:
            return apply();
      }
    }
    delay(50);
  }
}

inline bool checkTimeFormat(const Time& time)
{
  return (time.hour < 24 && time.hour >= 0 
    && time.minute < 60 && time.minute >= 0
    && time.second < 60 && time.second >= 0);

}

inline void shortBeep()
{
  digitalWrite(BUZZER, HIGH);
  delay(BEEP_SHORT_DELAY);
  digitalWrite(BUZZER, LOW);
}

inline void doubleShortBeep()
{
  digitalWrite(BUZZER, HIGH);
  delay(BEEP_SHORT_DELAY);
  digitalWrite(BUZZER, LOW);
  delay(BEEP_SHORT_DELAY / 3);
  digitalWrite(BUZZER, HIGH);
  delay(BEEP_SHORT_DELAY);
  digitalWrite(BUZZER, LOW);
}

inline void longBeep()
{
  digitalWrite(BUZZER, HIGH);
  delay(BEEP_LONG_DELAY);
  digitalWrite(BUZZER, LOW);
}

inline void showTime(const Time &time)
{
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(time.hour);
  lcd.print(":");
  lcd.print(time.minute);
  lcd.print(":");
  lcd.print(time.second);
}
