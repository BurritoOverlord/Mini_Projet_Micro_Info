#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <motors.h>
#include <audio_processing.h>
#include <sensors/proximity.h>

void moteur_manager_deplacement(uint8_t state_frequence)
{
	static uint8_t last_direction;
	if(state_frequence == 0 || state_frequence == 3 || state_frequence == 4)
		last_direction = state_frequence;

	if(state_frequence == 5)
		state_frequence = last_direction;

	for(int i = 0; i < 8; i++){
		if(get_calibrated_prox(i) > 600)
			state_frequence = 4;
	}

	switch(state_frequence){
		case 0://forward
			left_motor_set_speed(600);
			right_motor_set_speed(600);
			break;
		case 1://left
			left_motor_set_speed(-600);
			right_motor_set_speed(600);
			break;
		case 2://right
			left_motor_set_speed(600);
			right_motor_set_speed(-600);
			break;
		case 3://backward
			left_motor_set_speed(-600);
			right_motor_set_speed(-600);
			break;
		case 4://stop
			left_motor_set_speed(0);
			right_motor_set_speed(0);
			break;
	}


}
