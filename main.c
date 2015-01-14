//
//  main.c
//  Socket
//
//  Created by Yuki on 10/8/14.
//  Copyright (c) 2014 Yuki. All rights reserved.
//

#include <stdio.h>

/* global variables */
int i_am_a_global_variable = 999;

/* functions */

void another_function (int* a)
{
    (*a)++;
    i_am_a_global_variable += *a;
    return;
}

int some_function (int a)
{
    int some_local_var = a;
    printf("some_local_var has been changed to %d\n", some_local_var);
    another_function(&some_local_var);
    printf("some_local_var has been changed to %d\n", some_local_var);
    return 0;
}

int main (void)
{
    int number;
    printf ("enter number:");
    
    if (fscanf(stdin, "%d", &number) == 1) {
        some_function(number);
        printf("You’ve entered %d\n", number);
    } else {
        printf("No number entered. Bye.\n");
    }
    
    return 0;
}