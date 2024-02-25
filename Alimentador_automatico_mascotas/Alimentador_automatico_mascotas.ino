#include <Servo.h>

Servo servito;

const int trigPin = 6;
const int echoPin = 7;

int aperturasDeseadas = 4;  //aperturas dependiendo el usuario
int contadorAperturas = 0;

void setup() {
  servito.attach(9);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  int duration = pulseIn(echoPin, HIGH);
  int distancia = duration * 0.034 / 2;

  if (distancia < 30 && contadorAperturas < aperturasDeseadas) {
    servito.write(90); // Abre
    delay(700);
    servito.write(0); // cierra
    delay(700);
    servito.write(90);
    delay(700);
    contadorAperturas++;
  }
}