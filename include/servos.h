void initServos();
void setTargetPosAndSpeed(int servo_ind, int pos, int _speed);
void applyServoPos(int servo_ind, int pos);
void setTailSpeed(int _speed);                  
void core1_update_servos();
int getCurrentServoPos(int servo_ind);