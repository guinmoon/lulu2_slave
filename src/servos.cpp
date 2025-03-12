#include "servos.h"
#include "global_def.h"
#include <Servo.h>
#include "character.h"
#include "pico_rtc_utils.h"


Servo servo_left_front;
Servo servo_left_back;
Servo servo_right_front;
Servo servo_right_back;
Servo servo_tail;


int tailMovesCount = 0;
int tailMaxMovesCount = 6;

int currentPos[5] = {SERVO_90, 155, 155, SERVO_90, 0}; //sit position
int targetPos[5] = {SERVO_90, 155, 155, SERVO_90, SERVO_90}; //sit position
int servo_speed[5] = {1, 1, 1, 1, 0};
bool reverse_tail_move = false;

// int tail_speed = 1;

unsigned long lastUpdateTime = 0;

int getCurrentServoPos(int servo_ind){
    return(currentPos[servo_ind]);
}

void initServos()
{
    servo_left_front.attach(SER_LEFT_FRONT_PIN, USMIN, USMAX);
    servo_left_back.attach(SER_LEFT_BACK_PIN, USMIN, USMAX);
    servo_right_back.attach(SER_RIGHT_BACK_PIN, USMIN, USMAX);
    servo_right_front.attach(SER_RIGHT_FRONT_PIN, USMIN, USMAX);    
    servo_tail.attach(SER_TAIL_PIN, USMIN, USMAX);
}

void tailDetach(){
    servo_tail.detach();
}

void tailAttach(){
    servo_tail.attach(SER_TAIL_PIN, USMIN, USMAX);
}

void setTargetPosAndSpeed(int servo_ind, int pos, int _speed)
{
    targetPos[servo_ind] = constrain(pos, 0, 180);
    servo_speed[servo_ind] = constrain(_speed, SERVO_MIN_SPEED, SERVO_MAX_SPEED);
}

void applyServoPos(int servo_ind, int pos)
{
    // if (servo_speed[servo_ind]==0)
    //     return;
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
        // Serial.printf("Tail pos: %i\n",pos);
        break;
    }
}

void setTailSpeed(int _speed)
{
    servo_speed[SER_TAIL] = _speed;
}

void setTailMaxCount(int count){
    tailMaxMovesCount = count * 2;
}


void updateTail()
{
    if (servo_speed[SER_TAIL] == 0)
        return;
    if (tailMovesCount>=tailMaxMovesCount){
        servo_speed[SER_TAIL] = 0;
        tailMovesCount = 0;
        return;
    }
    if (!reverse_tail_move && currentPos[SER_TAIL] < SER_TAIL_MAX)
    {
        targetPos[SER_TAIL] = SER_TAIL_MAX;
    }
    if (!reverse_tail_move && currentPos[SER_TAIL] >= SER_TAIL_MAX)
    {
        reverse_tail_move = true;
        tailMovesCount++;
    }
    if (reverse_tail_move && currentPos[SER_TAIL] > SER_TAIL_MIN)
    {
        targetPos[SER_TAIL] = SER_TAIL_MIN;
    }
    if (reverse_tail_move && currentPos[SER_TAIL] <= SER_TAIL_MIN)
    {
        reverse_tail_move = false;
        tailMovesCount++;
    }
}

// Функция обновления сервоприводов
void updateServos()
{
    unsigned long currentTime = millis(); 
    if (currentTime - lastUpdateTime >= SER_UPDATE_INTERVAL)
    {
        if (currentTime-getLastPingTime()>=MSEC_UNTIL_SLEEP){
            setLastPingTime(currentTime);
            pico_sleep(RP_SLEEP_DURATION_SEC);
        }
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
            case SER_LEFT_FRONT:
                applyServoPos(SER_LEFT_FRONT, currentPos[i]);
                break;
            case SER_LEFT_BACK:
                applyServoPos(SER_LEFT_BACK, currentPos[i]);
                break;
            case SER_RIGHT_BACK:
                applyServoPos(SER_RIGHT_BACK, currentPos[i]);
                break;
            case SER_RIGHT_FRONT:
                applyServoPos(SER_RIGHT_FRONT, currentPos[i]);
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
        // sleep_ms(5);
    }
}
