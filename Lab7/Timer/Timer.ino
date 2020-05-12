int val;
volatile bool timer_on1 = false;
volatile bool timer_on2 = false;
volatile bool timer_show1 = true;
bool timer_sound1 = false;
bool timer_sound2 = false;

volatile unsigned long int timer_global1; //переменная подсчета миллисекунд
volatile unsigned long int timer_global2; //переменная подсчета миллисекунд
unsigned long int timer1 = 0;
unsigned long int timer2 = 0; //переменная подсчета миллисекунд

volatile unsigned long int timer_global3 = 0;
unsigned long int timer3 = 0;

bool last_buttom_check1 = false;
bool last_buttom_check2 = false;

volatile int thimer1_hour11 = 0;
volatile int thimer1_hour12 = 0;
volatile int thimer1_minute11 = 0;
volatile int thimer1_minute12 = 0;
volatile int thimer1_second11 = 0;
volatile int thimer1_second12 = 0;

volatile int thimer2_hour21 = 0;
volatile int thimer2_hour22 = 0;
volatile int thimer2_minute21 = 0;
volatile int thimer2_minute22 = 0;
volatile int thimer2_second21 = 0;
volatile int thimer2_second22 = 0;

int buttonA = 52;
int buttonB = 51;
int buttonC = 50;
int buttonD = 10;

int button_number = 0; // зміна для  номера кнопки
void setup()
{

  DDRA = B11111111;
  PORTA = B00111111;
  DDRF = B11111111;
  PORTF = B11000000;

  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);
  pinMode(buttonD, INPUT_PULLUP);
  pinMode(36, OUTPUT);
  DDRK = B00001111;
  PORTK = B11110000; // на вхsд с підтяжкою до + ( нулі на рядках)
  ////    lcd.begin(16, 2);                  // Задаем размерность экрана
  ////------ Timer2 ----------
  ASSR |= (1 << AS2);
  TIMSK2 |= (1 << TOIE2); // Режим CTC (сброс по совпадению)
  TCCR2B = (1 << CS22);   // Верхняя граница счета. Диапазон от 0 до 65535.

  sei();
  //------ Timer1 ----------
  //TCCR1B = (1<<WGM12);    // Режим CTC (сброс по совпадению)
  //OCR1A = 0xF9;           // Верхняя граница счета. Диапазон от 0 до 65535.
  //TIMSK1 = (1<<OCIE1A);   // Разрешить прерывание по совпадению
  //TCCR1B |= (1<<CS10)|(1<<CS11); // CLK/64
  //sei();
}

void seg7(int first, int second, int third, int four, int five, int six)
{
  int vertex[6] = {first, second, third, four, five, six};
  for (int i = 0; i < 6; i++)
  {
    int j = vertex[i];

    switch (i)
    {
    case 0:
      PORTA = B00000001;

      break;
    case 1:
      PORTA = B00000010;

      break;
    case 2:
      PORTA = B00000100;

      break;
    case 3:
      PORTA = B00001000;

      break;
    case 4:
      PORTA = B00010000;

      break;
    case 5:
      PORTA = B00100000;

      break;

    default:
      PORTA = B10000000;
      delay(1);
      break;
    }

    switch (j)
    {
    case 0:
      PORTF = B11000000;
      delay(1);
      break;
    case 1:
      PORTF = B11111001;
      delay(1);
      break;
    case 2:
      PORTF = B10100100;
      delay(1);
      break;
    case 3:
      PORTF = B10110000;
      delay(1);
      break;
    case 4:
      PORTF = B10011001;
      delay(1);
      break;
    case 5:
      PORTF = B10010010;
      delay(1);
      break;
    case 6:
      PORTF = B10000010;
      delay(1);
      break;
    case 7:
      PORTF = B11111000;
      delay(1);
      break;
    case 8:
      PORTF = B10000000;
      delay(1);
      break;
    case 9:
      PORTF = B10010000;
      delay(1);
      break;

    default:
      PORTF = B10000000;
      delay(1);
      break;
    }
    //delay(1);
  }

  //  PORTA = B00111111;
}

void showtimer(boolean wich_timer)
{
  if (wich_timer == true)
  {
    seg7(thimer1_hour12, thimer1_hour11, thimer1_minute12, thimer1_minute11, thimer1_second12, thimer1_second11);
  }
  else if (wich_timer == false)
  {
    seg7(thimer2_hour22, thimer2_hour21, thimer2_minute22, thimer2_minute21, thimer2_second22, thimer2_second21);
  }
}

ISR(TIMER2_OVF_vect)
//ISR (TIMER1_COMPA_vect)
{

  //int thimer1_hour11=0;
  //int thimer1_hour12=0;
  //int thimer1_minute11=0;
  //int thimer1_minute12=0;
  //int thimer1_second11=0;
  //int thimer1_second12=0;
  //  thimer2_minute21++;

  //  timer_global3++;
  //    if (timer_global3 - timer3 >= 1) {
  //      timer3 = timer_global3;
  //
  //
  //    }
  if (timer_on1 == true)
  {
    timer_global1++;
    if (timer_global1 - timer1 >= 2)
    {
      timer1 = timer_global1;

      if (thimer1_hour11 <= 0 && thimer1_hour12 <= 0 && thimer1_minute11 <= 0 && thimer1_minute12 <= 0 && thimer1_second11 <= 0 && thimer1_second12 <= 0)
      {
        timer_on1 = false;
        timer_sound1 = true;
        tone(36, 500, 5000);
      }
      else
      {

        thimer1_second11--;
        if (thimer1_second11 <= 0 && thimer1_second12 > 0)
        {

          thimer1_second12--;
          thimer1_second11 = 9;
        }

        if (thimer1_second12 <= 0 && thimer1_minute11 > 0)
        {

          thimer1_minute11--;
          thimer1_second12 = 6;
        }

        if (thimer1_minute11 <= 0 && thimer1_minute12 > 0)
        {

          thimer1_minute12--;
          thimer1_minute11 = 9;
        }

        if (thimer1_minute12 <= 0 && thimer1_hour11 > 0)
        {

          thimer1_hour11--;
          thimer1_minute12 = 6;
        }

        if (thimer1_hour11 <= 0 && thimer1_hour12 > 0)
        {

          thimer1_hour12--;
          thimer1_hour11 = 9;
        }
      }
    }
  }
  if (timer_on2 == true)
  {
    timer_global2++;
    if (timer_global2 - timer2 >= 2)
    {
      timer2 = timer_global2;
      if (thimer2_hour21 <= 0 && thimer2_hour22 <= 0 && thimer2_minute21 <= 0 && thimer2_minute22 <= 0 && thimer2_second21 <= 0 && thimer2_second22 <= 0)
      {
        timer_on2 = false;
        timer_sound2 = true;
        tone(36, 500, 5000);
      }
      else
      {

        //if(thimer1_second==0&&thimer1_minute>0)
        // {
        //  thimer1_minute--;
        //  thimer1_second=59;
        // }
        // if(thimer1_minute==0&&thimer1_hour>0)
        // {
        //  thimer1_hour--;
        //  thimer1_minute=59;
        // }

        thimer2_second21--;
        if (thimer2_second21 <= 0 && thimer2_second22 > 0)
        {

          thimer2_second22--;
          thimer2_second21 = 9;
        }

        if (thimer2_second22 <= 0 && thimer2_minute21 > 0)
        {

          thimer2_minute21--;
          thimer2_second22 = 6;
        }

        if (thimer2_minute21 <= 0 && thimer2_minute22 > 0)
        {

          thimer2_minute22--;
          thimer2_minute21 = 9;
        }

        if (thimer2_minute22 <= 0 && thimer2_hour21 > 0)
        {

          thimer2_hour21--;
          thimer2_minute22 = 6;
        }

        if (thimer2_hour21 <= 0 && thimer2_hour22 > 0)
        {

          thimer2_hour22--;
          thimer2_hour21 = 9;
        }
      }
    }
  }
}

void loop()
{
  showtimer(timer_show1);

  if (digitalRead(buttonA) == false)
  {
    delay(10);

    if (timer_show1 == true && timer_on1 == false)
    {
      thimer1_hour11++;
      if (thimer1_hour11 > 9)
      {
        thimer1_hour12++;
        thimer1_hour11 = 0;
      }
      if (thimer1_hour11 == 4 && thimer1_hour12 == 2)
      {
        thimer1_hour12 = 0;
        thimer1_hour11 = 0;
      }
    }

    if (timer_show1 == false && timer_on2 == false)
    {
      thimer2_hour21++;
      if (thimer2_hour21 > 9)
      {
        thimer2_hour22++;
        thimer2_hour21 = 0;
      }
      if (thimer2_hour21 == 4 && thimer2_hour22 == 2)
      {
        thimer2_hour22 = 0;
        thimer2_hour21 = 0;
      }
    }
  }
  if (digitalRead(buttonB) == false)
  {
    delay(10);
    if (timer_show1 == true && timer_on1 == false)
    {
      thimer1_minute11++;
      if (thimer1_minute11 > 9)
      {
        thimer1_minute12++;
        thimer1_minute11 = 0;
      }
      if (thimer1_minute11 == 0 && thimer1_minute12 == 6)
      {
        thimer1_minute11 = 0;
        thimer1_minute12 = 0;
      }
    }

    if (timer_show1 == false && timer_on2 == false)
    {
      thimer2_minute21++;
      if (thimer2_minute21 > 9)
      {
        thimer2_minute22++;
        thimer2_minute21 = 0;
      }
      if (thimer2_minute21 == 0 && thimer2_minute22 == 6)
      {
        thimer2_minute21 = 0;
        thimer2_minute22 = 0;
      }
    }
  }

  if (digitalRead(buttonC) == false)
  {
    delay(10);
    if (timer_show1 == true)
    {
      timer_show1 = false;
    }
    else if (timer_show1 == false)
    {
      timer_show1 = true;
    }
  }
  if (digitalRead(buttonD) == false)
  {
    delay(10);
    if (timer_show1 == true)
    {
      timer_on1 = true;
      if (last_buttom_check1 == true)
      {
        timer_on1 = false;
        last_buttom_check1 = false;
        thimer1_hour11 = 0;
        thimer1_hour12 = 0;
        thimer1_minute11 = 0;
        thimer1_minute12 = 0;
        thimer1_second11 = 0;
        thimer1_second12 = 0;
      }
      else if (last_buttom_check1 == false)
      {
        last_buttom_check1 = true;
      }
    }

    if (timer_show1 == false)
    {
      timer_on2 = true;
      if (last_buttom_check2 == true)
      {
        timer_on2 = false;
        last_buttom_check2 = false;

        thimer2_hour21 = 0;
        thimer2_hour22 = 0;
        thimer2_minute21 = 0;
        thimer2_minute22 = 0;
        thimer2_second21 = 0;
        thimer2_second22 = 0;
      }
      else if (last_buttom_check2 == false)
      {
        last_buttom_check2 = true;
      }
    }
  }
}



