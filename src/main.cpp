
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


void dogInit()
{
    // setTailSpeed(4);
    // layDown(4);    
    
    // sitDown(5);
    // joke1(4);
    giveHand(5,true);
    // stand(4);    
    // leftHand(4);
    // sitDown(5);
    // delay(1000);
    // stand(2);
    // delay(1000);
    // dance1(5);
    // halfLayDown(4);    
    // leftHand(4,false);
    // delay(2000);
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
    
    // leftHand(5,false);
    // stepBack(4,2);
    // joke1(7);
   
    // stepForward(4,3);
    // tailLegsStand(4);
    // fullLayDown(4);
    // delay(2000);
    
    // layDown(4);
    // leftHand(5,false);
    // Настройка I2C
    // Serial.println("Go to sleep after 5 sec");
    // delay(5000);
    setLastPingTime(millis());
    // prepareSleep();
    Wire.begin(8);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestHandler);
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
