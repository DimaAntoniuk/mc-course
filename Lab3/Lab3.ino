#include <LiquidCrystal.h>

int time = 0;

LiquidCrystal lcd(59, 58, 57, 56, 55, 54);

void setup()
{
  lcd.begin(16, 2);
  lcd.print("Facker Display");
}

void loop()
{
  lcd.setCursor(0,1);
  lcd.print(time);
  delay(1000);
  time++;
}
