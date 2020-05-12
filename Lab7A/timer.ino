#include <Arduino.h>

#define DELAY 400
#define BUZZER 18
#define BUTTON1 47
#define BUTTON3 49
#define BUTTON5 38
#define BUTTON8 40
#define LEDS_NUM 8

const int leds[] = {22, 23, 24, 25, 26, 27, 28, 29};

bool isAlarm = false, isDisabled = false;

ISR(TIMER1_COMPA_vect)
{
    if (flag1)
    {
        //Algorithm1();
        Algorithm5();
    }
    if (flag2)
    {
        //Algorithm3();
        Algorithm8();
    }
 
}

void setup()
{
    int flag1 = false;
    int flag2 = false;

    for (auto led : leds)
    {
        pinMode(led, OUTPUT);
    }
    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON3, INPUT_PULLUP);
    DDRA = B11111111;
    PORTA = B00000000;
    DDRF = B11111111;
    PORTF = B00000000;
    pinMode(BUTTON5, INPUT_PULLUP);
    pinMode(BUTTON8, INPUT_PULLUP);

    // timer
    noInterrupts();

    pinMode(BUZZER, OUTPUT);

    TCCR1A = WGM10;
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); //CTC mode & Prescaler @ 1024
    TIMSK1 = (1 << OCIE1A);                            // дозвіл на переривання по співпадінню
    OCR1A = 0x3D08;                                    // 1 sec (16MHz AVR)

    interrupts();
    //
}

void loop()
{

    // // ALG1
    // if (BUTTON1 == false)
    // {
    //     flag1 = true;
    // }
    // // ALG3
    // if (BUTTON3 == false)
    // {
    //     flag2 = true;
    // }

    // ALG5
    if (BUTTON5 == false)
    {
        flag1 = true;
    }
    // ALG8
    if (BUTTON8 == false)
    {
        flag2 = true;
    }
}

inline void Algorithm1()
{
    for (int i = 0; i < LEDS_NUM; i++)
    {
        digitalWrite(leds[i], HIGH);
        delay(100);
        digitalWrite(leds[i], LOW);
        delay(DELAY);
    }
}

inline void Algorithm3()
{
    for (int i = 0; i < LEDS_NUM / 2; i++)
    {
        digitalWrite(leds[i], HIGH);
        digitalWrite(leds[LEDS_NUM - i - 1], HIGH);
        delay(200);
        digitalWrite(leds[i], LOW);
        digitalWrite(leds[LEDS_NUM - i - 1], LOW);
        delay(DELAY);
    }
}

inline void Algorithm5()
{
    for (int j = 0; j < 2; j++)
    {
        for (int i = j; i < 8; i = i + 2)
        {
            PORTA |= 1 << i;
            delay(550);
            PORTA &= ~(1 << i);
        }
    }
}

inline void Algorithm8()
{
    int k = 7;
    for (int j = 0; j < 4; j++)
    {
        PORTF |= 1 << k;
        delay(550);
        PORTF &= ~(1 << k);

        PORTF |= 1 << j;
        delay(550);
        PORTF &= ~(1 << j);
        k--;
    }
}