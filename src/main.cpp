
#include "pico/multicore.h"
#include <Wire.h>
#include "commands.h"
#include "global_def.h"
#include "servos.h"
#include "character.h"




void dogInit()
{
    setTailSpeed(4);
    layDown(3);
    waitForServoPos(SER_LEFT_FRONT, SERVO_180,1000);
    delay(1000);
    stand(4);
    waitForServoPos(SER_LEFT_FRONT, SERVO_90,1000);
    // leftHand(4);
    // sitDown(5);
    // delay(1000);
    // stand(2);
    // delay(1000);
    // dance1(5);
    // halfLayDown(4);
    setTailSpeed(0);
    // delay(1000);
    // happy(4);
}


void setup()
{

    // Настройка сервоприводов
    initServos();
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
