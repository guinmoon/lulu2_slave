void onReceive(int howMany);
void sitDown(int _speed);
void halfLayDown(int _speed );
void tailLegsStand(int _speed );
// void halfLayDownTail(int _speed );
void fullLayDown(int _speed);
void layDown(int _speed );
void stand(int _speed);
void jump(int _speed );
void happy(int _speed);
void dance1(int _speed);
void giveHand(int _speed, bool leftHand );
void dance_elem2(int _speed);
void dance_elem1(int _speed);
void waitForServoPos(int servo_ind, int wait_angle,int timeout);
void stepForward(int _speed , int count);
void stepBack(int _speed, int count);
void setTailSpeedAndCount(int speed);
long getLastPingTime();
void setLastPingTime(long t);
void joke1(int _speed );
void giveHandLong(int _speed, bool leftHand );
void onRequest();