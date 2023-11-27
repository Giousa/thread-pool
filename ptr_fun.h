//
// Created by 张蒙蒙 on 2023/11/24.
//

#ifndef THREAD_POOL_PTR_FUN_H
#define THREAD_POOL_PTR_FUN_H

#include <iostream>

using namespace std;

int add(int, int);

int subtract(int, int);

int multiply(int, int);

int divide(int, int);

//int (*calculate)(int, int);


int getResult(int, int, int (*calculate)(int, int));

void test_fun();


#endif //THREAD_POOL_PTR_FUN_H
