#define VCC       2
#define TRIGGER   3
#define ECHO      4
#define GND       5

void setup() {
  pinMode(VCC, OUTPUT);
  pinMode(GND, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(VCC, HIGH);
}

void sendchar(char x) {
  for (int i = 7; i >= 0; i--) {
    sendbit((x >> i) & 1);
  }
}

void sendbit(char x) {
  unsigned long d = 0;

  if (x == 0) {
    d = 700;
  }

  if (x == 1) {
    d = 2300;
  }

  digitalWrite(13, HIGH);
  for (int i = 0; i < d; i ++) {
    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(100);
  }
  digitalWrite(13, LOW);

  delay(300);
}

void loop() {
  sendchar('H');
  delay(700);
  sendchar('e');
  delay(700);
  sendchar('l');
  delay(700);
  sendchar('l');
  delay(700);
  sendchar('o');
  delay(700);
  sendchar('\n');
  delay(700);
}
