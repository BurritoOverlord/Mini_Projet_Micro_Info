#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void moteur_manager_deplacement(uint16_t state_frequence)
{
	switch(state_frequence){
		case 0:
			left_motor_set_speed(600);
			right_motor_set_speed(600);
			break;
		case 1:
			left_motor_set_speed(-600);
			right_motor_set_speed(600);
			break;
		case 2:
			left_motor_set_speed(600);
			right_motor_set_speed(-600);
			break;
		case 3:
			left_motor_set_speed(-600);
			right_motor_set_speed(-600);
			break;
		case 4:
			left_motor_set_speed(0);
			right_motor_set_speed(0);
			break;
	}
}
