#define BA 37
#define BB 36
#define BC 35
#define BD 34
#define BUZZER 30
#define BEEP_SHORT_DELAY 50
#define BEEP_LONG_DELAY 250

bool isAlarm = false, isDisabled = false, showAlarm = false;

struct Time
{
  uint8_t second, minute, hour;
};
Time time = {0, 0, 0};
Time alarm = {0, 0, 0};

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

  if (!isDisabled && time.hour == alarm.hour && time.minute == alarm.minute && time.second == alarm.second)
  {
    isAlarm = true;
  }
}

inline void seg7ShowTime(Time &time);
inline void seg7ShowAlarm(Time &time);
inline void onPressA();
inline void onPressB();
inline void onPressC();
inline void onPressD();
inline void shortBeep();
inline void doubleShortBeep();
inline void longBeep();
inline void playAlarm();

void setup()
{
  noInterrupts();

  DDRB= B11111111;
   PORTB = B01111110;
   DDRF= B11111111;
   PORTA =  B11000000;
  

  pinMode(BA, INPUT_PULLUP);
  pinMode(BB, INPUT_PULLUP);
  pinMode(BC, INPUT_PULLUP);
  pinMode(BD, INPUT_PULLUP);
  
  pinMode(BUZZER, OUTPUT);

  TCCR1A = WGM10;
  TCCR1B = (1 << WGM11) | (1 << CS12) | (1 << CS10); //CTC mode & Prescaler @ 1024
  TIMSK1 = (1 << OCIE1A);                            // дозвіл на переривання по співпадінню
  OCR1A = 0x3D08;                                    // 1 sec (16MHz AVR)
  
  // TCCR0A |= (1 << WGM01);
  // OCR0A = 0xF9;
  // TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect
  // sei();         //enable interrupts
  // TCCR0B |= (1 << CS02);

  interrupts();
}

void loop()
{

  if (digitalRead(BA) == false)
  {
    delay(10);
    shortBeep();
    onPressA(time);
  }
  if (digitalRead(BB) == false)
  {
    delay(10);
    shortBeep();
    onPressB(time);
  }
  if (digitalRead(BC) == false)
  {
    delay(10);
    shortBeep();
    onPressC();
  }
  
  seg7ShowTime(time);

  delay(10);

  if (isAlarm)
  {
    playAlarm();
  }
}


inline void onPressA(Time &time)
{
  time.hour++;
  if (time.hour == 24)
  {
    time.hour = 0;
  }
}

inline void onPressB(Time &time)
{
  time.minute++;
  if (time.minute == 60)
  {
    time.minute = 0;
  }
}

inline void onPressC()
{
  seg7ShowAlarm(alarm);
  while (true)
  {
    if (digitalRead(BA) == false)
    {
      delay(10);
      shortBeep();
      alarm.hour++;
      if (alarm.hour == 24)
      {
        alarm.hour = 0;
      }
      seg7ShowAlarm(alarm);
    }
    if (digitalRead(BB) == false)
    {
      delay(10);
      shortBeep();
      alarm.minute++;
      if (alarm.minute == 60)
      {
        alarm.minute = 0;
      }
      seg7ShowAlarm(alarm);
    }
    if (digitalRead(BC) == false)
    {
      delay(10);
      shortBeep();
      break;
    }
    if (digitalRead(BD) == false)
    {
      delay(10);
      shortBeep();
      onPressD();
      seg7ShowAlarm(alarm);
    }
  }
  doubleShortBeep();
}

inline void onPressD()
{
  isDisabled = !isDisabled;
}

inline void playAlarm()
{
  uint8_t iteration = 0;

  while (iteration < 10)
  {
    longBeep();
    iteration++;
    delay(5);
  }
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

void seg7ShowTime(Time &time)
{
  int vertex[6] = {
      time.hour / 10, time.hour % 10,
      time.minute / 10, time.minute % 10,
      time.second / 10, time.second % 10};
  for (int i = 0; i < 6; i++)
  {
    int j = vertex[i];

    switch (i)
    {
    case 0:
      PORTB = B00000010;

      break;
    case 1:
      PORTB = B00000100;

      break;
    case 2:
      PORTB = B00001000;

      break;
    case 3:
      PORTB = B00010000;

      break;
    case 4:
      PORTB = B00100000;

      break;
    case 5:
      PORTB = B01000000;

      break;

    default:
      PORTB = B10000000;
      delay(1);
      break;
    }
switch (j)
    {
    case 0:
      PORTA = B11000000;
      delay(1);
      break;
    case 1:
      PORTA = B11111001;
      delay(1);
      break;
    case 2:
      PORTA = B10100100;
      delay(1);
      break;
    case 3:
      PORTA = B10110000;
      delay(1);
      break;
    case 4:
      PORTA = B10011001;
      delay(1);
      break;
    case 5:
      PORTA = B10010010;
      delay(1);
      break;
    case 6:
      PORTA = B10000010;
      delay(1);
      break;
    case 7:
      PORTA = B11111000;
      delay(1);
      break;
    case 8:
      PORTA = B10000000;
      delay(1);
      break;
    case 9:
      PORTA = B10010000;
      delay(1);
      break;

    default:
      PORTA = B10000000;
      delay(1);
      break;
    }
    delay(2);
  }
}

void seg7ShowAlarm(Time &time)
{
  int vertex[4] = {
      time.hour / 10, time.hour % 10,
      time.minute / 10, time.minute % 10};
  for (int i = 0; i < 4; i++)
  {
    int j = vertex[i];

    switch (i)
    {
    case 0:
      PORTB = B00001000;

      break;
    case 1:
      PORTB = B00010000;

      break;
    case 2:
      PORTB = B00100000;

      break;
    case 3:
      PORTB = B01000000;

      break;

    default:
      PORTB = B10000000;
      delay(1);
      break;
    }

    switch (j)
    {
    case 0:
      PORTA = B11000000;
      delay(1);
      break;
    case 1:
      PORTA = B11111001;
      delay(1);
      break;
    case 2:
      PORTA = B10100100;
      delay(1);
      break;
    case 3:
      PORTA = B10110000;
      delay(1);
      break;
    case 4:
      PORTA = B10011001;
      delay(1);
      break;
    case 5:
      PORTA = B10010010;
      delay(1);
      break;
    case 6:
      PORTA = B10000010;
      delay(1);
      break;
    case 7:
      PORTA = B11111000;
      delay(1);
      break;
    case 8:
      PORTA = B10000000;
      delay(1);
      break;
    case 9:
      PORTA = B10010000;
      delay(1);
      break;

    default:
      PORTA = B10000000;
      delay(1);
      break;
    }
  }
  if (!isDisabled) {
    PORTB = B00000010;
    delay(10);
    PORTA = B10001000;
    delay(10);
  }
}