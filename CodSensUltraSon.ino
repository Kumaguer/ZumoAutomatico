// Pines de sensores IR
const int sensorIzq = 21;
const int sensorDer = 19;

// Pines del sensor ultrasónico
const int trigPin = 14;
const int echoPin = 26;

// Pines del puente H
const int IN1 = 22;
const int IN2 = 23;
const int IN3 = 2;
const int IN4 = 4;
const int ENA = 5;
const int ENB = 18;

// Variables de PWM
const int periodoPWM = 16666; // ~60 Hz
int dutyENA = 255;
int dutyENB = 255;

// Variables para lógica
enum EstadoMovimiento { AVANZAR, RETROCEDER, DETENER, GIRAR };
EstadoMovimiento ultimoEstado = DETENER;

void setup() {
  Serial.begin(921600);

  pinMode(sensorIzq, INPUT);
  pinMode(sensorDer, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
  int valIzq = digitalRead(sensorIzq);
  int valDer = digitalRead(sensorDer);

  // Si sensores IR están activos, tienen prioridad
  if (valIzq == HIGH) {
    avanzar();
    ultimoEstado = AVANZAR;
  } 
  else if (valDer == HIGH) {
    retroceder();
    ultimoEstado = RETROCEDER;
  } 
  else {
    // Lógica del sensor ultrasónico
    long duration;
    float distance;

    // Disparo del pulso ultrasónico
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Medición del eco
    duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
    if (duration > 0) {
      distance = duration * 0.034 / 2;
      Serial.print("Distancia: ");
      Serial.print(distance);
      Serial.println(" cm");

      if (distance < 20.0) {
        avanzar();
        ultimoEstado = AVANZAR;
      } else {
        girar();
        ultimoEstado = GIRAR;
      }
    } else {
      Serial.println("Sensor fuera de rango");
      girar();
      ultimoEstado = GIRAR;
    }
  }

  // PWM manual
  int tiempoHighENA = map(dutyENA, 0, 255, 0, periodoPWM);
  int tiempoHighENB = map(dutyENB, 0, 255, 0, periodoPWM);

  digitalWrite(ENA, HIGH);
  delayMicroseconds(tiempoHighENA);
  digitalWrite(ENA, LOW);
  delayMicroseconds(periodoPWM - tiempoHighENA);

  digitalWrite(ENB, HIGH);
  delayMicroseconds(tiempoHighENB);
  digitalWrite(ENB, LOW);
  delayMicroseconds(periodoPWM - tiempoHighENB);

  delay(20); // Pequeño retardo para estabilidad
}

// === Funciones de movimiento ===
void avanzar() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  dutyENA = 255;
  dutyENB = 255;
}

void retroceder() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  dutyENA = 255;
  dutyENB = 255;
}

void detener() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  dutyENA = 0;
  dutyENB = 0;
}

void girar() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  dutyENA = 255;
  dutyENB = 255;
}