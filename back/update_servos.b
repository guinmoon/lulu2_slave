// Функция обновления сервоприводов
void updateServos()
{
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime >= 15)
    {
        lastUpdateTime = currentTime;

        for (int i = 0; i < 4; i++)
        {
            if (currentPos[i] < targetPos[i])
            {
                currentPos[i] += speed[i];
                if (currentPos[i] > targetPos[i])
                    currentPos[i] = targetPos[i];
            }
            else if (currentPos[i] > targetPos[i])
            {
                currentPos[i] -= speed[i];
                if (currentPos[i] < targetPos[i])
                    currentPos[i] = targetPos[i];
            }

            switch (i)
            {
            case 0:
                setServoPos(0,currentPos[i]);
                break;
            case 1:
                setServoPos(1,currentPos[i]);
                break;
            case 2:
                setServoPos(2,currentPos[i]);
                break;
            case 3:
                setServoPos(3,currentPos[i]);
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