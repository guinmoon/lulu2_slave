void tailThread()
{
    while (true)
    {
        if (tail_speed == 0)
        {
            delay(200);
            continue;
        }
        int _delay = constrain((50 - tail_speed * 5), 1, 50);
        for (int i = 0; i < 160; i += 10)
        {
            setServoPos(4, i);
            delay(_delay);
        }
        for (int i = 160; i > 0; i -= 10)
        {
            setServoPos(4, i);
            delay(_delay);
        }
    }
}