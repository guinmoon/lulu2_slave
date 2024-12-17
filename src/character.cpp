#include "character.h"
#include "commands.h"
#include "global_def.h"
#include "servos.h"
#include <Wire.h>

bool doingAction = false;

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
        if (doingAction)
            return;
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
        if (commandID == COMMAND_DANCE1)
        {
            int commandArg = Wire.read();
            dance1(commandArg);
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

void jump(int _speed = 10)
{
    applyServoPos(0, SERVO_90);
    applyServoPos(3, SERVO_90);
    applyServoPos(1, 0);
    applyServoPos(2, 0);
}

void happy(int _speed)
{
    int delay_d = 800 - _speed * 50;
    stand(_speed);
    waitForServoPos(SER_LEFT_BACK, 90, delay_d);
    waitForServoPos(SER_LEFT_FRONT, 90, delay_d);
    dance1_elem2(_speed);
    dance1_elem2(_speed);
}

void waitForServoPos(int servo_ind, int wait_angle, int timeout)
{
    int iter_num = 0;
    while (getCurrentServoPos(servo_ind) != wait_angle)
    {
        delay(SER_UPDATE_INTERVAL);
        iter_num++;
        if (iter_num * SER_UPDATE_INTERVAL > timeout)
            break;
    }
}

void dance1_elem1(int _speed)
{
    int p1 = 45;
    int p2 = 145;
    int delay_d = 800 - _speed * 50;

    setTargetPosAndSpeed(SER_LEFT_BACK, p1, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, p1, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, p2, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, p2, _speed);

    // delay(800 - _speed * 50);
    waitForServoPos(SER_LEFT_BACK, p1, delay_d);

    setTargetPosAndSpeed(SER_LEFT_BACK, p2, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, p2, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, p1, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, p1, _speed);
    // delay(800 - _speed * 50);
    waitForServoPos(SER_LEFT_BACK, p2, delay_d);
}

void dance1_elem2(int _speed)
{
    int p1 = 50;
    int p2 = 140;
    int delay_d = 800 - _speed * 50;
    setTargetPosAndSpeed(SER_LEFT_BACK, p1, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, p1, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, p1, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, p1, _speed);

    waitForServoPos(SER_LEFT_FRONT, p1, delay_d);

    setTargetPosAndSpeed(SER_LEFT_BACK, p2, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, p2, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, p2, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, p2, _speed);

    waitForServoPos(SER_LEFT_FRONT, p2, delay_d);
}

void dance1(int _speed)
{
    int delay_d = 800 - _speed * 50;
    stand(_speed);
    waitForServoPos(SER_LEFT_BACK, 90, delay_d);
    waitForServoPos(SER_LEFT_FRONT, 90, delay_d);

    dance1_elem1(_speed);
    dance1_elem1(_speed);
    delay(_speed * 5);
    dance1_elem2(_speed);
    dance1_elem2(_speed);
    delay(_speed * 5);
    dance1_elem1(_speed);
    dance1_elem1(_speed);
    delay(_speed * 5);
    dance1_elem2(_speed);
    dance1_elem2(_speed);

    stand(_speed);
    waitForServoPos(SER_LEFT_BACK, 90, delay_d);
    waitForServoPos(SER_LEFT_FRONT, 90, delay_d);
}

void leftHand(int _speed = 10)
{
    int p1 = 80;
    int p2 = 180;

    int delay_d = 800 - _speed * 50;
    sitDown(_speed);
    waitForServoPos(SER_LEFT_FRONT, SERVO_90, delay_d);

    setTargetPosAndSpeed(SER_LEFT_FRONT, 80, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, 100, _speed);
    setTargetPosAndSpeed(SER_LEFT_BACK, 120, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, 100, _speed);
    waitForServoPos(SER_RIGHT_FRONT, 100, delay_d);
    delay(_speed * 5);

    setTargetPosAndSpeed(SER_LEFT_FRONT, p1, _speed);
    waitForServoPos(SER_LEFT_FRONT, p1, delay_d);
    delay(_speed * 5);
    setTargetPosAndSpeed(SER_LEFT_FRONT, p2, _speed);
    waitForServoPos(SER_LEFT_FRONT, p2, delay_d);
    delay(_speed * 5);
    setTargetPosAndSpeed(SER_LEFT_FRONT, p1, _speed);
    waitForServoPos(SER_LEFT_FRONT, p1, delay_d);
    delay(_speed * 5);
    setTargetPosAndSpeed(SER_LEFT_FRONT, p2, _speed);
    waitForServoPos(SER_LEFT_FRONT, p2, delay_d);
    delay(_speed * 5);
    setTargetPosAndSpeed(SER_LEFT_FRONT, p1, _speed);
    waitForServoPos(SER_LEFT_FRONT, p1, delay_d);
    delay(_speed * 5);
    setTargetPosAndSpeed(SER_LEFT_FRONT, p2, _speed);
    sitDown(_speed);
    waitForServoPos(SER_LEFT_FRONT, SERVO_90, delay_d);
}

void angry()
{
}
