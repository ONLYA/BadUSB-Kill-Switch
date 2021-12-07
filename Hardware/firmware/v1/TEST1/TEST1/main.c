/*
 * TEST1.c
 *
 * Created: 16/09/2020 02:27:46
 * Author : User
 */ 


#include "sam.h"


int main(void)
{
    /* Replace with your application code */
	PORT->Group[0].DIRSET.reg = PORT_PA00;
    while (1) 
    {
		PORT->Group[0].OUTSET.reg = PORT_PA00;
    }
}
