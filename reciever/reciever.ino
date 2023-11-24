#define VCC       2
#define TRIGGER   3
#define ECHO      4
#define GND       5

const unsigned long THRESHOLD = 20;
const unsigned long END_DURATION = 55;
int state = 0;

const int numSamples = 7;
long sensorValues[numSamples];
int currentIndex = 0;
long pulseduration = 0;
long silentduration = 0;
unsigned long duration;
unsigned long silent;

char data;

void setup() {
  for (int i = 0; i < numSamples; ++i) {
    sensorValues[i] = 0;
  }

  state = 0;
  silent = 0;
  data = 0;

  Serial.begin(9600);

  pinMode(VCC, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(GND, OUTPUT);
  digitalWrite(VCC, HIGH);
}

void loop() {
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
  noInterrupts();
  long latestValue = pulseIn(ECHO, HIGH, 25000);
  interrupts();
  sensorValues[currentIndex] = latestValue;
  currentIndex = (currentIndex + 1) % numSamples;
  long sum = 0;
  for (int i = 0; i < numSamples; ++i) {
    sum += sensorValues[i];
  }
  long movingAverage = sum / numSamples;
  bool analog = movingAverage > 50;
  // Serial.println(analog);

  if (state == 0 && analog) {
    duration = 0;
    state = 1;  // Transition to high state
  } else if (state == 1 && !analog) {
    silent = 0;

    if (duration < THRESHOLD) {
      // Short duration, consider it as '0'
      data = (data << 1);
    } else if (duration > THRESHOLD) {
      // Long duration, consider it as '1'
      data = (data << 1) | 1;
    }
    state = 0;
  } else if (silent > END_DURATION) {
    //    Serial.println(data, BIN);
    if (('0' <= data && data <= '9') ||
        ('A' <= data && data <= 'Z') ||
        ('a' <= data && data <= 'z') ||
        (data == '\n')) {
      Serial.print(data);
    }
    else {
      Serial.println("Demodulation failed!");
    }

    silent = 0;
    data = 0;
  }
  duration ++;
  silent ++;
  delayMicroseconds(10);
}
