//
// Created by 张蒙蒙 on 2023/11/24.
//

#include "ptr_fun.h"


int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    return a / b;
}

int getResult(int a, int b, int (*calculate)(int, int)){
    return calculate(a , b);
}

void test_fun() {
    cout << "add >>> " << getResult(2,3,add) << endl;
    cout << "subtract >>> " << getResult(2,3,subtract) << endl;
    cout << "multiply >>> " << getResult(2,3,multiply) << endl;
    cout << "divide >>> " << getResult(2,3,divide) << endl;
}

