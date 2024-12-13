#include <Servo.h>
#include "pico/multicore.h"
#include <Wire.h>
#include "commands.h"
#include "servos_conf.h"

Servo servo_left_front;
Servo servo_left_back;
Servo servo_right_front;
Servo servo_right_back;
Servo servo_tail;

int currentPos[5] = {SERVO_90, SERVO_90, SERVO_90, SERVO_90, 0};
int targetPos[5] = {SERVO_90, SERVO_90, SERVO_90, SERVO_90, SERVO_90};
int servo_speed[5] = {1, 1, 1, 1, 0};
bool reverse_tail_move = false;

int tail_speed = 1;

unsigned long lastUpdateTime = 0;

void setTargetPosAndSpeed(int servo_ind, int pos, int _speed)
{
    targetPos[servo_ind] = constrain(pos, 0, SERVO_180);
    servo_speed[servo_ind] = constrain(_speed, 1, 10);
}

void applyServoPos(int servo_ind, int pos)
{
    switch (servo_ind)
    {
    case SER_LEFT_FRONT:
        servo_left_front.write(pos);
        // servo_left_front.writeMicroseconds(2400);
        break;
    case SER_LEFT_BACK:
        servo_left_back.write(pos);
        break;
    case SER_RIGHT_FRONT:
        servo_right_front.write(180 - pos);
        break;
    case SER_RIGHT_BACK:
        servo_right_back.write(180 - pos);
        break;
    case SER_TAIL:
        servo_tail.write(pos);
        break;
    }
}

void setTailSpeed(int _speed)
{
    servo_speed[SER_TAIL] = _speed;
}

void updateTail()
{
    if (!reverse_tail_move && currentPos[SER_TAIL] < 160)
    {
        targetPos[SER_TAIL] = 160;
    }
    if (!reverse_tail_move && currentPos[SER_TAIL] >= 160)
    {
        reverse_tail_move = true;
    }
    if (reverse_tail_move && currentPos[SER_TAIL] > 20)
    {
        targetPos[SER_TAIL] = 20;
    }
    if (reverse_tail_move && currentPos[SER_TAIL] <= 20)
    {
        reverse_tail_move = false;
    }
}

// Функция обновления сервоприводов
void updateServos()
{
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime >= 15)
    {
        lastUpdateTime = currentTime;
        updateTail();
        for (int i = 0; i < 5; i++)
        {
            if (currentPos[i] < targetPos[i])
            {
                currentPos[i] += servo_speed[i];
                if (currentPos[i] > targetPos[i])
                    currentPos[i] = targetPos[i];
            }
            else if (currentPos[i] > targetPos[i])
            {
                currentPos[i] -= servo_speed[i];
                if (currentPos[i] < targetPos[i])
                    currentPos[i] = targetPos[i];
            }

            switch (i)
            {
            case 0:
                applyServoPos(SER_LEFT_FRONT, currentPos[i]);
                break;
            case 1:
                applyServoPos(1, currentPos[i]);
                break;
            case 2:
                applyServoPos(2, currentPos[i]);
                break;
            case 3:
                applyServoPos(3, currentPos[i]);
                break;
            case SER_TAIL:
                applyServoPos(SER_TAIL, currentPos[i]);
                break;
            }
        }
    }
}

// Функция, работающая на втором ядре
void core1_update_servos()
{
    while (true)
    {
        // Обновление сервоприводов выполняется на втором ядре
        updateServos();
    }
}

void sitDown(int _speed = 10)
{
    setTargetPosAndSpeed(SER_LEFT_BACK, 155, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, 155, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, SERVO_90, _speed);
}

void halfLayDown(int _speed = 10)
{
    setTargetPosAndSpeed(SER_LEFT_BACK, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, 170, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, 170, _speed);
}

void layDown(int _speed = 10)
{
    setTargetPosAndSpeed(SER_LEFT_BACK, SERVO_180, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, SERVO_180, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, SERVO_180, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, SERVO_180, _speed);
}

void stand(int _speed = 10)
{
    setTargetPosAndSpeed(SER_LEFT_BACK, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, SERVO_90, _speed);
}

void jump()
{
    applyServoPos(0, SERVO_90);
    applyServoPos(3, SERVO_90);
    applyServoPos(1, 0);
    applyServoPos(2, 0);
}

void happy(int _speed)
{

    setTargetPosAndSpeed(SER_LEFT_BACK, SERVO_90, 10);
    setTargetPosAndSpeed(SER_RIGHT_BACK, SERVO_90, 10);
    setTargetPosAndSpeed(SER_LEFT_FRONT, SERVO_90, 10);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, SERVO_90, 10);

    setTargetPosAndSpeed(SER_LEFT_BACK, 30, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, 30, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, 30, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, 30, _speed);

    delay(800 - _speed * 50);

    setTargetPosAndSpeed(SER_LEFT_BACK, 150, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, 150, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, 150, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, 150, _speed);

    delay(800 - _speed * 50);

    setTargetPosAndSpeed(SER_LEFT_BACK, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, SERVO_90, _speed);
}

void leftHand(int _speed = 10)
{
    sitDown(_speed);
    delay(800 - _speed * 50);
    setTargetPosAndSpeed(SER_LEFT_FRONT, 80, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, 100, _speed);
    setTargetPosAndSpeed(SER_LEFT_BACK, 120, _speed);
    delay(600 - _speed * 50);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, 100, _speed);
    delay(600 - _speed * 50);
    setTargetPosAndSpeed(SER_LEFT_FRONT, 80, _speed);
    delay(600 - _speed * 50);
    setTargetPosAndSpeed(SER_LEFT_FRONT, 180, _speed);
    delay(600 - _speed * 50);
    setTargetPosAndSpeed(SER_LEFT_FRONT, 80, _speed);
    delay(600 - _speed * 50);
    setTargetPosAndSpeed(SER_LEFT_FRONT, 180, _speed);
    delay(600 - _speed * 50);
    setTargetPosAndSpeed(SER_LEFT_FRONT, 90, _speed);
}

void angry()
{
}

// Функция обработки полученных данных
void receiveEvent(int howMany)
{
    char buf[200];
    Serial.println("recived");
    while (Wire.available() >= 1)
    {
        // int servoID = Wire.read();
        // int pos = Wire.read();
        // int spd = Wire.read();
        int commandID = Wire.read();
        if (commandID == COMMAND_STAND)
        {
            int commandArg = Wire.read();
            stand(commandArg);
            sprintf(buf, "Recived: %i %i", commandID, commandArg);
        }
        if (commandID == COMMAND_SIT)
        {
            int commandArg = Wire.read();
            sprintf(buf, "Recived: %i %i", commandID, commandArg);
            sitDown(commandArg);
        }
        if (commandID == COMMAND_LAYDOWN)
        {
            int commandArg = Wire.read();
            sprintf(buf, "Recived: %i %i", commandID, commandArg);
            layDown(commandArg);
        }
        if (commandID == COMMAND_SET_TAIL_SPEED)
        {
            int commandArg = Wire.read();
            setTailSpeed(commandArg);
            sprintf(buf, "Recived: %i %i", commandID, commandArg);
        }
        if (commandID == COMMAND_LEFTHAND)
        {
            int commandArg = Wire.read();
            leftHand(commandArg);
            sprintf(buf, "Recived: %i %i", commandID, commandArg);
        }
        if (commandID == COMMAND_HAPPY)
        {
            int commandArg = Wire.read();
            happy(commandArg);
            sprintf(buf, "Recived: %i %i", commandID, commandArg);
        }
        if (commandID == COMMAND_HALFLAYDOWN)
        {
            int commandArg = Wire.read();
            halfLayDown(commandArg);
            sprintf(buf, "Recived: %i %i", commandID, commandArg);
        }
        Serial.println(buf);
        // if (servoID >= 0 && servoID < 4)
        // {
        //     targetPos[servoID] = constrain(pos, 0, SERVO_180);
        //     speed[servoID] = constrain(spd, 1, 10);
        // }
    }
}

void dogInit()
{
    setTailSpeed(4);
    stand();
    delay(1000);
    sitDown(5);
    delay(1000);
    stand(2);
    delay(1000);
    // halfLayDown(4);
    setTailSpeed(0);
    // delay(1000);
    // happy(4);
}

void setup()
{

    // Настройка сервоприводов

    servo_left_front.attach(6, USMIN, USMAX);
    servo_left_back.attach(3, USMIN, USMAX);
    servo_right_front.attach(8, USMIN, USMAX);
    servo_right_back.attach(2, USMIN, USMAX);
    servo_tail.attach(7, USMIN, USMAX);

    multicore_launch_core1(core1_update_servos);

    dogInit();

    // delay(2000);
    // layDown(4);
    // leftHand(5);
    // Настройка I2C

    Wire.begin(8);
    Wire.onReceive(receiveEvent);
}

void loop()
{
    // Основной петля на первом ядре может быть использована для других задач
    // Например, для получения и обработки данных через I2C

    // setTargetPosAndSpeed(SER_LEFT_FRONT,SERVO_90,10);
    // setTargetPosAndSpeed(SER_RIGHT_FRONT,SERVO_90,10);
    // setTargetPosAndSpeed(SER_LEFT_BACK,SERVO_90,10);
    // setTargetPosAndSpeed(SER_RIGHT_BACK,SERVO_90,10);
    Serial.println("loop");
    delay(1000);
}
