
#include "pico/multicore.h"
#include <Wire.h>
#include "commands.h"
#include "global_def.h"
#include "servos.h"
#include "character.h"
#include "pico/stdlib.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
// #include <stdlib.h>
#include "hardware/clocks.h"
#include "pico_rtc_utils.h"
#include <hardware/rtc.h>
#include "hardware/structs/scb.h"





/// Determine sleep duration and enter Deep Sleep Mode
void prepareSleep(void)
{
    unsigned long sleep_interval = 5;
    Serial.printf("Shutdown() - sleeping for %lu s\n", sleep_interval);
    pico_sleep(sleep_interval);
    Serial.println("Awake");
    // rp2040.restart();
}

void dogInit()
{
    // setTailSpeed(4);
    layDown(3);
    waitForServoPos(SER_LEFT_FRONT, SERVO_180, 1000);
    delay(1000);
    stand(4);
    waitForServoPos(SER_LEFT_FRONT, SERVO_90, 1000);
    // leftHand(4);
    // sitDown(5);
    // delay(1000);
    // stand(2);
    // delay(1000);
    // dance1(5);
    // halfLayDown(4);    
    // leftHand(4,false);
    delay(2000);
    // setTailSpeed(0);
    // delay(1000);
    // happy(4);
}

void setup()
{

    // Настройка сервоприводов
    initServos();
    multicore_launch_core1(core1_update_servos);

    dogInit();
    // stepForward(4,3);
    // tailLegsStand(4);
    // fullLayDown(4);
    // delay(2000);
    
    // layDown(4);
    // leftHand(5);
    // Настройка I2C
    Serial.println("Go to sleep after 5 sec");
    delay(5000);
    prepareSleep();
    Wire.begin(8);
    Wire.onReceive(receiveEvent);
}

void loop()
{
    // Основной петля на первом ядре может быть использована для других задач
    // Например, для получения и обработки данных через I2C

    // setTargetPosAndSpeed(SER_RIGHT_FRONT,90,5);
    // setTargetPosAndSpeed(SER_RIGHT_BACK,90,5);
    // setTargetPosAndSpeed(SER_LEFT_FRONT,90,5);
    // setTargetPosAndSpeed(SER_LEFT_BACK,90,5);
    //

    // setTargetPosAndSpeed(SER_TAIL,90,5);
    // setTailSpeed(4);
    Serial.println("loop");
    delay(1000);
}
