const int leverPin = 2;
const int ledPin = 7;
const int speakerPin = 12;

int happinessLevel = 100;
int happinessDecaySpeed = 1;

unsigned long previousMillis = 0;
const long interval = 1000; // Interval for decreasing happiness 
const int initialPulseDuration = 5000; // Initial pulse duration (5 seconds)
int pulseDuration = initialPulseDuration; // Current pulse duration

bool isPulsing = false; // Flag to track if pulsing is ongoing
unsigned long pulseStartTime = 0; // Time when pulsing started

bool isLeverTurned = false;

void setup() {
  Serial.begin(9600);
  pinMode(leverPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  int leverState = digitalRead(leverPin);

  digitalWrite(ledPin, LOW);
  digitalWrite(speakerPin, LOW);

  if (leverState == LOW && !isLeverTurned) {
    ResetHappiness(true);
  }
  else if(leverState == HIGH && isLeverTurned){
    ResetHappiness(false);
  }

  CalculateHappiness();
  CalculateHardwareAttentionNeed();
}

void ResetHappiness(bool isTurned){
  Serial.println("Happiness received");
    happinessLevel = 100; 
    isLeverTurned = isTurned;
    noTone(speakerPin);
}

void CalculateHardwareAttentionNeed() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  }

  if (happinessLevel <= 0) {
      // Send shutdown signal if attention level depleted
      Serial.println("shutdown");
  }

  if (isPulsing) {
    // Check if pulse duration has elapsed
    if (currentMillis - pulseStartTime >= pulseDuration) {
      isPulsing = false; // Stop pulsing
      digitalWrite(ledPin, LOW);
    }
  } else {
    if (happinessLevel <= 50) {
      pulseDuration = map(happinessLevel, 0, 50, 200, 5000);
      isPulsing = true;
      pulseStartTime = currentMillis;
      digitalWrite(ledPin, HIGH);
      tone(speakerPin, 1000, 1000);
    }
  }

  Serial.print("Current Millis: ");
  Serial.print(currentMillis);
  Serial.print(" Pulse Start Time: ");
  Serial.print(pulseStartTime);
  Serial.print(" Pulse Duration: ");
  Serial.println(pulseDuration);
}

void CalculateHappiness() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    happinessLevel -= happinessDecaySpeed;
    Serial.print("Happiness level: ");
    Serial.println(happinessLevel);
  }
}
