#ifndef __MYOS__MYSYSCALLS_H
#define __MYOS__MYSYSCALLS_H

#include <common/types.h>
#include <multitasking.h>

int thread_create(myos::Task* newTask);
void thread_yield();
bool thread_join(int threadID);
void thread_exit();

#endif
