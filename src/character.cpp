#include "character.h"
#include "commands.h"
#include "global_def.h"
#include "servos.h"
#include <Wire.h>
#include "pico_rtc_utils.h"

bool doingAction = false;

long lastPingTime = 0;

// Функция обработки полученных данных
void receiveEvent(int howMany)
{
    char buf[200];
    Serial.println("recived");
    lastPingTime = millis();
    // return;
    while (Wire.available() >= 1)
    {
        if (doingAction)
            return;
        int commandID = Wire.read();
        int commandArg = Wire.read();
        sprintf(buf, "Recived: %i %i", commandID, commandArg);
        Serial.println(buf);
        if (commandArg < 0)
        {
            Serial.println("WRONG COMMAND");
            return;
        }
        if (commandID == COMMAND_STAND)
        {
            stand(commandArg);
        }
        if (commandID == COMMAND_SIT)
        {
            sitDown(commandArg);
        }
        if (commandID == COMMAND_LAYDOWN)
        {
            layDown(commandArg);
        }
        if (commandID == COMMAND_SET_TAIL_SPEED)
        {
            setTailSpeedAndCount(commandArg);
        }
        if (commandID == COMMAND_LEFTHAND)
        {
            leftHand(commandArg, true);
        }
        if (commandID == COMMAND_RIGHTHAND)
        {
            leftHand(commandArg, false);
        }
        if (commandID == COMMAND_HAPPY)
        {
            happy(commandArg);
        }
        if (commandID == COMMAND_HALFLAYDOWN)
        {
            halfLayDown(commandArg);
        }
        if (commandID == COMMAND_HALFLAYDOWNTAIL)
        {
            halfLayDown(commandArg);
        }
        if (commandID == COMMAND_FULLLAYDOWN)
        {
            fullLayDown(commandArg);
        }
        if (commandID == COMMAND_TAILLEGSSTAND)
        {
            tailLegsStand(commandArg);
        }
        if (commandID == COMMAND_DANCE1)
        {
            dance1(commandArg);
        }
        if (commandID == DANCE_ELEM1)
        {
            dance_elem1(commandArg);
        }
        if (commandID == DANCE_ELEM2)
        {
            dance_elem2(commandArg);
        }
        if (commandID == RP_SYS_COMMAND_PING)
        {
            lastPingTime = millis();
        }
        if (commandID == RP_SYS_COMMAND_WAKE)
        {
            recover_from_sleep();
        }
        if (commandID == RP_SYS_COMMAND_SLEEP)
        {
            setLastPingTime(millis());
            pico_sleep(RP_SLEEP_DURATION_SEC);
        }
    }
}

long getLastPingTime()
{
    return lastPingTime;
}

void setLastPingTime(long t)
{
    lastPingTime = t;
}

void setTailSpeedAndCount(int speed)
{
    if (speed == 0)
    {
        tailDetach();
    }
    else
    {
        tailAttach();
    }
    switch (speed)
    {
    case 2:
        setTailMaxCount(3);
        break;
    case 3:
        setTailMaxCount(4);
        break;
    case 4:
        setTailMaxCount(5);
        break;
    case 5:
        setTailMaxCount(5);
        break;
    case 6:
        setTailMaxCount(7);
        break;
    case 7:
        setTailMaxCount(8);
        break;
    case 8:
        setTailMaxCount(8);
        break;
    case 9:
        setTailMaxCount(8);
        break;

    default:
        setTailMaxCount(3);
        break;
    }
    setTailSpeed(speed);
}

void beginCommand()
{
    doingAction = true;
}

void endCommand(int _speed, int ser, int angle)
{
    // int delay_d = 800 - _speed * 50;
    // waitForServoPos(ser, angle, delay_d);
    doingAction = false;
}

void sitDown(int _speed = 10)
{
    int delay_d = 800 - _speed * 50;
    beginCommand();
    setTargetPosAndSpeed(SER_LEFT_BACK, 155, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, 155, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, SERVO_90, _speed);
    waitForServoPos(SER_LEFT_BACK, 155, delay_d);
    endCommand(_speed, SER_RIGHT_BACK, 155);
}

void halfLayDown(int _speed = 10)
{
    int delay_d = 800 - _speed * 50;
    beginCommand();
    setTargetPosAndSpeed(SER_LEFT_BACK, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, 170, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, 170, _speed);
    waitForServoPos(SER_LEFT_FRONT, 170, delay_d);
    endCommand(_speed, SER_RIGHT_BACK, SERVO_90);
}

void tailLegsStand(int _speed = 10)
{
    beginCommand();
    int delay_d = 800 - _speed * 50;
    setTargetPosAndSpeed(SER_LEFT_BACK, 30, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, 30, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, SERVO_90, _speed);
    waitForServoPos(SER_LEFT_FRONT, SERVO_90, delay_d);
    endCommand(_speed, -1, -1);
    // delay(2000);
    // sitDown(_speed);
}

// void halfLayDownTail(int _speed = 10)
// {
//     int delay_d = 800 - _speed * 50;
//     setTargetPosAndSpeed(SER_LEFT_BACK, 0, _speed);
//     setTargetPosAndSpeed(SER_RIGHT_BACK, 0, _speed);
//     setTargetPosAndSpeed(SER_LEFT_FRONT, SERVO_90, _speed);
//     setTargetPosAndSpeed(SER_RIGHT_FRONT, SERVO_90, _speed);
//     delay(2000);
//     sitDown(_speed);
// }

void fullLayDown(int _speed = 10)
{
    int delay_d = 800 - _speed * 50;
    beginCommand();
    setTargetPosAndSpeed(SER_LEFT_BACK, 0, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, 0, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, SERVO_180, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, SERVO_180, _speed);
    waitForServoPos(SER_LEFT_FRONT, SERVO_180, delay_d);
    endCommand(_speed, SER_RIGHT_BACK, 0);
}

void layDown(int _speed = 10)
{
    int delay_d = 800 - _speed * 50;
    beginCommand();
    setTargetPosAndSpeed(SER_LEFT_BACK, SERVO_180, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, SERVO_180, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, SERVO_180, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, SERVO_180, _speed);
    waitForServoPos(SER_LEFT_BACK, SERVO_180, delay_d);
    endCommand(_speed, SER_RIGHT_BACK, SERVO_180);
}

void stand(int _speed = 10)
{
    int delay_d = 800 - _speed * 50;
    beginCommand();
    setTargetPosAndSpeed(SER_LEFT_BACK, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_RIGHT_BACK, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, SERVO_90, _speed);
    waitForServoPos(SER_LEFT_BACK, SERVO_90, delay_d);
    waitForServoPos(SER_LEFT_FRONT, SERVO_90, delay_d);
    endCommand(_speed, SER_RIGHT_BACK, SERVO_90);
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
    doingAction = true;
    int delay_d = 800 - _speed * 50;
    stand(_speed); 
    dance_elem2(_speed);
    dance_elem2(_speed);
    doingAction = false;
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

void dance_elem1(int _speed)
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
    waitForServoPos(SER_RIGHT_FRONT, p1, delay_d);
}

void dance_elem2(int _speed)
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
    doingAction = true;
    int delay_d = 800 - _speed * 50;
    stand(_speed);


    dance_elem1(_speed);
    dance_elem1(_speed);
    delay(_speed * 5);
    dance_elem2(_speed);
    dance_elem2(_speed);
    delay(_speed * 5);
    dance_elem1(_speed);
    dance_elem1(_speed);
    delay(_speed * 5);
    dance_elem2(_speed);
    dance_elem2(_speed);

    stand(_speed);

    doingAction = false;
}

void leftHand(int _speed = 10, bool leftHand = true)
{
    doingAction = true;
    int p1 = 80;
    int p2 = 180;

    int hand1 = SER_LEFT_FRONT;
    int hand2 = SER_RIGHT_FRONT;
    if (!leftHand)
    {
        hand1 = SER_RIGHT_FRONT;
        hand2 = SER_LEFT_FRONT;
    }

    int delay_d = 800 - _speed * 50;
    setTargetPosAndSpeed(SER_LEFT_BACK, 145, _speed); 
    if (leftHand)   
        setTargetPosAndSpeed(SER_RIGHT_BACK, 150, _speed);
    else
        setTargetPosAndSpeed(SER_LEFT_BACK, 150, _speed);
    setTargetPosAndSpeed(SER_LEFT_FRONT, SERVO_90, _speed);
    setTargetPosAndSpeed(SER_RIGHT_FRONT, SERVO_90, _speed);
    waitForServoPos(SER_LEFT_BACK, 145, delay_d);

    setTargetPosAndSpeed(hand1, 80, _speed);
    setTargetPosAndSpeed(hand2, 100, _speed);
    // setTargetPosAndSpeed(hand1, 120, _speed);
    setTargetPosAndSpeed(hand2, 100, _speed);
    waitForServoPos(hand2, 100, delay_d);
    delay(_speed * 5);

    setTargetPosAndSpeed(hand1, p1, _speed);
    waitForServoPos(hand1, p1, delay_d);
    delay(_speed * 5);
    setTargetPosAndSpeed(hand1, p2, _speed);
    waitForServoPos(hand1, p2, delay_d);
    delay(_speed * 5);
    setTargetPosAndSpeed(hand1, p1, _speed);
    waitForServoPos(hand1, p1, delay_d);
    delay(_speed * 5);
    setTargetPosAndSpeed(hand1, p2, _speed);
    waitForServoPos(hand1, p2, delay_d);
    delay(_speed * 5);
    setTargetPosAndSpeed(hand1, p1, _speed);
    waitForServoPos(hand1, p1, delay_d);
    delay(_speed * 5);
    setTargetPosAndSpeed(hand1, p2, _speed);
    sitDown(_speed);
    waitForServoPos(hand1, SERVO_90, delay_d);
    doingAction = false;
}

void stepForward(int _speed = 10, int count = 1)
{
    doingAction = true;
    int delay_d = 800 - _speed * 50;
    for (int i = 0; i < count; i++)
    {
        layDown(_speed);
        waitForServoPos(SER_RIGHT_FRONT, SERVO_180, delay_d);
        waitForServoPos(SER_RIGHT_BACK, SERVO_180, delay_d);
        halfLayDown(_speed);
        // setTargetPosAndSpeed(SER_LEFT_FRONT, SERVO_90, _speed);
        // setTargetPosAndSpeed(SER_RIGHT_FRONT, SERVO_90, _speed);
        waitForServoPos(SER_LEFT_BACK, SERVO_90, delay_d);
    }
    doingAction = false;
}

// void stepBack(int _speed = 10, int count = 1)
// {
//     doingAction = true;
//     int delay_d = 800 - _speed * 50;
//     fullLayDown(5);
//     waitForServoPos(SER_LEFT_BACK, 0, delay_d);
//     for (int i = 0; i < count; i++)
//     {
//         setTargetPosAndSpeed(SER_RIGHT_FRONT, 0, _speed);
//         setTargetPosAndSpeed(SER_LEFT_FRONT, 0, _speed);
//         // setTargetPosAndSpeed(SER_RIGHT_BACK, 0, _speed);
//         // setTargetPosAndSpeed(SER_LEFT_BACK, 0, _speed);
//         waitForServoPos(SER_RIGHT_FRONT, 0, delay_d);
//         setTargetPosAndSpeed(SER_RIGHT_FRONT, 80, _speed);
//         setTargetPosAndSpeed(SER_LEFT_FRONT, 80, _speed);
//         waitForServoPos(SER_RIGHT_FRONT, 80, delay_d);
//         // setTargetPosAndSpeed(SER_RIGHT_BACK, SERVO_90, _speed);
//         // setTargetPosAndSpeed(SER_LEFT_BACK, SERVO_90, _speed);
        
//         delay(500);
//     }
//     doingAction = false;
// }

void joke1(int _speed = 10)
{
    doingAction = true;
    int delay_d = 800 - _speed * 50;
    stand(_speed);    
    setTargetPosAndSpeed(SER_LEFT_FRONT, 180, _speed);
    setTargetPosAndSpeed(SER_LEFT_BACK, 0, _speed);
    doingAction = false;
}

void angry()
{
}
