#include "Counter.h"
void Counter_init(Counter *this, int count_max, int count_min){
    this->count_max = count_max;
    this->count_min = count_min;
    this->count = count_min;
}
void Counter_increment(Counter *this){
    if(this->count < this->count_max){
        this->count++;
    }
}
void Counter_decrement(Counter *this){
    if(this->count > this->count_min){
        this->count--;
    }
}
inline int Counter_get(Counter *this){
    return this->count;
}
void Counter_set(Counter *this, int count){
    if(count > this->count_max){
        this->count = this->count_max;
    }else if(count < this->count_min){
        this->count = this->count_min;
    }else{
        this->count = count;
    }
}