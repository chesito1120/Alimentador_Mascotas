#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>

#define BUZZER_PIN 10 // Pin del buzzer
#define TRIGGER_PIN 3 // Pin del trigger 
#define ECHO_PIN 4    // Pin del echo 

RTC_DS3231 rtc;
Servo Serivito;

const int numHorarios = 3;
int horarios[numHorarios][2] = {
    {10, 32},  // alarma 1
    {10, 33},  // alarma 2
    {10, 34}   // alarma 3
};

bool servoActivado[numHorarios] = {false}; // Variable para que nomas se active una vez 

void setup()
{
    Serial.begin(9600);
    Serivito.attach(9); // pin del servo 
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    if (!rtc.begin())
    {
        Serial.println("¡No se encontro el módulo de reloj!");
        while (1);
    }

    if (rtc.lostPower())
    {
        Serial.println("RTC perdió la alimentación, ajustando la hora...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

void loop()
{
    DateTime now = rtc.now();

    Serial.print("Hora actual: ");
    printDateTime(now);

    
    if (now.second() % 3 == 0)
    {
        int distancia = medirDistancia();
        Serial.print("Distancia: ");
        Serial.println(distancia);
        if (distancia > 20)
        {
            // distancia pa pitidos
            for (int i = 0; i < 2; i++)
            {
                digitalWrite(BUZZER_PIN, HIGH);
                delay(200);
                digitalWrite(BUZZER_PIN, LOW);
                delay(200);
            }
        }
    }

    for (int i = 0; i < numHorarios; i++)
    {
        if (now.hour() == horarios[i][0] && now.minute() == horarios[i][1] && !servoActivado[i])
        {
            Serial.println("Abriendo la compuerta...");
            Serivito.write(180);           
            digitalWrite(BUZZER_PIN, HIGH); 
            delay(2000);                    
            Serial.println("Cerrando la compuerta...");
            Serivito.write(0);              
            digitalWrite(BUZZER_PIN, LOW);  
            servoActivado[i] = true;        
        }
    }

    delay(1000); 
}

void printDateTime(DateTime dt)
{
    Serial.print(dt.year(), DEC);
    Serial.print('/');
    Serial.print(dt.month(), DEC);
    Serial.print('/');
    Serial.print(dt.day(), DEC);
    Serial.print(" ");
    Serial.print(dt.hour(), DEC);
    Serial.print(':');
    Serial.print(dt.minute(), DEC);
    Serial.print(':');
    Serial.print(dt.second(), DEC);
    Serial.println();
}

int medirDistancia()
{
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    unsigned long tiempo = pulseIn(ECHO_PIN, HIGH);
    int distancia = tiempo * 0.034 / 2; //formuleichon pa la distancia
    return distancia;
}
