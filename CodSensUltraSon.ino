const int trigPin = 4;
const int echoPin = 5;

volatile long duration = 0;
volatile bool echoReceived = false;

void IRAM_ATTR echoISR() {
  static unsigned long startTime = 0;
  
  if (digitalRead(echoPin) == HIGH) {
    startTime = micros();
  } else {
    duration = micros() - startTime;
    echoReceived = true;
  }
}

void setup() {
  Serial.begin(921600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(echoPin), echoISR, CHANGE);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Espera activa mínima mientras llega el eco
  unsigned long startWait = millis();
  while (!echoReceived && (millis() - startWait) < 50);

  if (echoReceived) {
    echoReceived = false;
    float distance = duration * 0.034 / 2;
    Serial.print("Distancia: ");
    Serial.print(distance);
    Serial.println(" cm");
  } else {
    Serial.println("Sensor fuera de rango");
  }

  delay(20);  // Menos retardo para respuesta rápida
}