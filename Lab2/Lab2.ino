#define DELAY 400
#define BUTTON1 12
#define BUTTON2 11
#define LEDS_NUM 8

const int leds[] = {3, 4, 5, 6, 7, 8, 9, 10};

inline void sendAlgorithm1();
inline void showAlgorithm1();
inline void sendAlgorithm2();
inline void showAlgorithm2();

void setup() {
  for(auto led: leds) {
    pinMode(led, OUTPUT);
  }
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    auto algorithmMode = Serial.read();
    if (algorithmMode == '1') {
      showAlgorithm1();
    } else {
      showAlgorithm2();
    }
  }
  if (digitalRead(BUTTON1)) {
    sendAlgorithm1();
    delay(50);
  }
  if (digitalRead(BUTTON2)) {
    sendAlgorithm2();
    delay(50);
  }
}

inline void showAlgorithm1() {
  for(int i = 0; i<LEDS_NUM; i++) {
    digitalWrite(leds[i], HIGH);
    delay(100);
    digitalWrite(leds[i], LOW);
    delay(DELAY);
  }
}

inline void showAlgorithm2() {
  for(int i = 0; i<LEDS_NUM/2; i++) {
    digitalWrite(leds[i], HIGH);
    digitalWrite(leds[LEDS_NUM-i-1], HIGH);
    delay(200);
    digitalWrite(leds[i], LOW);
    digitalWrite(leds[LEDS_NUM-i-1], LOW);
    delay(DELAY);
  }
}

inline void sendAlgorithm1() {
  String secret_message = "";
  for(int i = 0; i<LEDS_NUM; i++) {
    secret_message += String(leds[i]) + ",";
  }
  secret_message += String(DELAY) + ",";
  secret_message += String(1);
  Serial.println(secret_message);
}

inline void sendAlgorithm2() {
  String secret_message = "";
  for(int i = 0; i<LEDS_NUM/2; i++) {
    secret_message += String(leds[i]) + ",";
    secret_message += String(leds[LEDS_NUM - i - 1]) + ",";
  }
  secret_message += String(DELAY) + ",";
  secret_message += String(2);
  Serial.println(secret_message);
}
