#include "ringbuffer.h"
#include <stdio.h>
#include <iostream>

//测试程序：ringbuffer空间大小为10，存入15个数据，会造成5个数据被新数据覆盖而消失
//读的时候依然是从最早未被覆盖的数据开始读，仍然维持先进先出
int main(){
    ringbuffer<int> ringbuffer(10);
    int data[15];
    for(int i = 0; i < 15; i++){
        data[i] = i;
    }
    ringbuffer.write(data, 15);

    for(int i = 0; i < 10; i++){
        std::cout << ringbuffer.getNum(i) << std::endl;
    }

    int nums[10];
    for(int i = 0; i < 10; i++){
        ringbuffer.read(nums, 10);
    }

    std::cout << "读完后ringbuffer剩余数据数为：" << ringbuffer.getOccupied() << std::endl;

    std::cout << "读出的数据依次为：" << std::endl;

    for(int i = 0; i < 10; i++){
        std::cout << nums[i] << std::endl;
    }

    return 0;
}