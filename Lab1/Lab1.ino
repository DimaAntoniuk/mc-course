int buttonCheck = 0;
int button = 47;

void setup() {
  for (int i = 22; i <= 29; i++) {
      pinMode(i, OUTPUT);
  }  
  pinMode(button, INPUT);
}

void loop() {
  buttonCheck = digitalRead(button);
  if(buttonCheck == HIGH) {
    for(int i = 22; i<=29; i++) {
      digitalWrite(i, HIGH);
      delay(100);
      digitalWrite(i, LOW);
      delay(400);
    }
  }
}
