#include "Counter.h"
void Counter_init(Counter *this, int count_max, int count_min,int step){
    this->count_max = count_max;
    this->count_min = count_min;
    this->count = count_min;
    this->step = step;
}
void Counter_increment(Counter *this){
    if(this->count + this->step >= this->count_max){
        this->count = this->count_max;
    }else{
        this->count += this->step;
    }
}

void Counter_increment_circle(Counter *this){
    if(this->count + this->step > this->count_max){
        this->count = this->count_min;
    }else{
        this->count += this->step;
    }
}

void Counter_decrement(Counter *this){
    if(this->count - this->step <= this->count_min){
        this->count = this->count_min;
    }else{
        this->count -= this->step;
    }
}

void Counter_decrement_circle(Counter *this){
    if(this->count - this->step < this->count_min){
        this->count = this->count_max;
    }else{
        this->count -= this->step;
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

inline void Counter_reset(Counter *this){
    this->count = this->count_min;
}

inline bool Counter_unreach_max(Counter *this){
    return this->count < this->count_max;
}
inline bool Counter_unreach_min(Counter *this){
    return this->count > this->count_min;
}

inline bool Counter_exceed_or_reach_max(Counter *this){
    return this->count >= this->count_max;
}

inline bool Counter_exceed_or_reach_min(Counter *this){
    return this->count <= this->count_min;
}

bool Counter_set_cnt(Counter *this, int count){
    if(count > this->count_max){
        this->count = this->count_max;
        return false;
    }else if(count < this->count_min){
        this->count = this->count_min;
        return false;
    }else{
        this->count = count;
        return true;
    }
}