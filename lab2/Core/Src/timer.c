#include "timer.h"
#include "i"
#include "i2c_oled.h"
/*
刷新显示
传入 0 -> 无按键按下
传入 1 -> 开始计时  
传入 2 -> 停止计时
传入 3 -> 清零计时 ∂
传入 4 -> 暂停计时 
*/
void timer(int a)
{
    switch(a)
    {
        case 1:
            begin = 1;
            break;
        case 2:
            stop = 1;
            break;
        case 3:
            zero = 1;
            break;
        case 4:
            pause = 1;
            break;
        default:
            break;
    }
    if(zero == 1){
        time = 0;
        zero = 0;
    }
    if(stop == 1){
        stop = 0;
        begin = 0;
    }
    if(begin == 1 && stop == 0){
        OLED_ShowNumber_16(0, 0, time);
    }else{
        OLED_ShowStr(0,0,"press to start",2);
    }

}