#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
// #include <SoftwareSerial.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Текущие позиции сервоприводов
int currentPos[4] = {0, 0, 0, 0};

// Целевые позиции
int targetPos[4] = {90, 90, 90, 90};

// Скорости для каждого сервопривода
int speed[4] = {1, 1, 1, 1};

unsigned long lastUpdateTime = 0;

void updateServo(int id)
{
    if (currentPos[id] < targetPos[id])
    {
        currentPos[id] += speed[id];
        if (currentPos[id] > targetPos[id])
            currentPos[id] = targetPos[id];
    }
    else if (currentPos[id] > targetPos[id])
    {
        currentPos[id] -= speed[id];
        if (currentPos[id] < targetPos[id])
            currentPos[id] = targetPos[id];
    }

    //   switch (id) {
    //     case 0: servo1.write(currentPos[id]); break;
    //     case 1: servo2.write(currentPos[id]); break;
    //     case 2: servo3.write(currentPos[id]); break;
    //     case 3: servo4.write(currentPos[id]); break;
    //   }
}

void receiveEvent(int howMany)
{
    Serial.println("On Recive:");
    char buf[200];    
    while (Wire.available() >= 3)
    {
        int servoID = Wire.read();
        int pos = Wire.read();
        int spd = Wire.read();
        sprintf(buf, "Recived: %i %i %i", servoID, pos, spd);
        Serial.println(buf);
        if (servoID >= 0 && servoID < 4)
        {
            targetPos[servoID] = constrain(pos, 0, 180);
            speed[servoID] = constrain(spd, 1, 10); // Ограничение скорости между 1 и 10
        }
    }
}


void sendEvent()
{
    Wire.print(100);
    // Wire.println("hi");
}

// SoftwareSerial SUART(D3, D4); 

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello");
    //   Wire.begin(5,4,8); // I2C адрес 8
    //   Wire.begin(4,5,8); // I2C адрес 8
    // Wire.pins(5, 4);
    // Wire.begin(D3,D4,14); // I2C адрес 8
    // Wire.setSCL(5);
    // Wire.setSDA(4);
    Wire.begin(8); // I2C адрес 8
    // Wire1.begin();
    Wire.onReceive(receiveEvent);
    // Wire.onRequest(sendEvent);

    //   servo1.attach(3);
    //   servo2.attach(5);
    //   servo3.attach(6);
    //   servo4.attach(9);
    // SUART.begin(9600);
    // Инициализация сервоприводов в начальные позиции
    for (int i = 0; i < 4; i++)
    {
        updateServo(i);
    }
    Serial.println("Initialized.");
}

void loop()
{
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime >= 15)
    {
        lastUpdateTime = currentTime;

        for (int i = 0; i < 4; i++)
        {
            updateServo(i);
        }
    }
    delay(1000);
    Serial.println("loop.");
}