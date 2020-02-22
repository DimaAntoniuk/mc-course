int buttonCheck = 0;
int button = 2;

void setup() {
  for (int i = 3; i <= 10; i++) {
      pinMode(i, OUTPUT);
  }  
  pinMode(button, INPUT);
}

void loop() {
  buttonCheck = digitalRead(button);
  if(buttonCheck == HIGH) {
    for(int i = 3; i<=10; i++) {
      digitalWrite(i, HIGH);
      delay(100);
      digitalWrite(i, LOW);
      delay(400);
    }
  }
}
