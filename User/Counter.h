#ifndef _COUNTER_H_
#define _COUNTER_H_
#include <stdbool.h>
typedef struct Counter_
{
    int count;
    int count_max;
    int count_min;
    int step;
} Counter;

void Counter_init(Counter *this, int count_max, int count_min,int step);
void Counter_increment(Counter *this);
void Counter_decrement(Counter *this);
void Counter_increment_circle(Counter *this);
void Counter_decrement_circle(Counter *this);
int Counter_get(Counter *this);
void Counter_reset(Counter *this);
void Counter_set(Counter *this, int count);
bool Counter_unreach_max(Counter *this);
bool Counter_unreach_min(Counter *this);
bool Counter_exceed_or_reach_max(Counter *this);
bool Counter_exceed_or_reach_min(Counter *this);
bool Counter_set_cnt(Counter *this, int count);
#define CounterGET(this) ((this)->count)
#define CounterSET(this, count) ((this)->count = (count))
#define CounterINC(this) ((this)->count++)
#define CounterDEC(this) ((this)->count--)

#endif // _COUNTER_H_