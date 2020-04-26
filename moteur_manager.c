#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <arm_math.h>

#include <main.h>
#include <usbcfg.h>
#include <chprintf.h>

#include <motors.h>
#include <audio_processing.h>
#include <sensors/proximity.h>
#include <moteur_manager.h>
#include <button.h>
#include <spi_comm.h>
#include <leds.h>

#define WHEEL_DISTANCE		5.32f//[cm]
#define WHEEL_PERIMETRE		13//[cm]
#define NSTEP_ONE_TURN 		1000
#define PERIMETRE_EPUCK		(PI*WHEEL_DISTANCE)

void moteur_manager_deplacement(uint8_t state_frequence)
{
	static uint8_t last_direction;
	static uint8_t last_state_frequence;
	static bool recording = false;
	static bool play_record = false;
	static bool turning = false;
	static bool data_reading = false;
	static uint8_t rec_data[6000];//l'enregistrement ne peut pas être plus long que 1 minutes (10ms * 6000 = 60s)
	static uint8_t rec_num = 0;

	//static int32_t position_to_reach_r = -PERIMETRE_EPUCK/2 * NSTEP_ONE_TURN/WHEEL_PERIMETRE;
	//static int32_t position_to_reach_l = PERIMETRE_EPUCK/2 * NSTEP_ONE_TURN/WHEEL_PERIMETRE;

	if(toggle_record())
	{
		if(recording){
			recording = false;
			play_record = true;
			set_led(LED1, 0);
			set_led(LED3, 1);
			set_led(LED7, 1);
		}
		else
		{
			recording = true;
			set_led(LED1, 1);
		}
	}

	//enregistrement des données dans rec_data.
	if(recording){
		rec_data[rec_num] = state_frequence;
		rec_num++;
	}

	if(play_record){
		if(rec_num != 0)
		{
			rec_num--;
			state_frequence = rec_data[rec_num];
		}
		else
		{
			play_record = false;
			rec_num = 0;
			state_frequence = 4;
			set_led(LED3, 0);
			set_led(LED7, 0);
		}
		/*
		if(!turning && !data_reading)
		{
			 left_motor_set_pos(0);
			 right_motor_set_pos(0);
			 turning = true;
		}
		else if(turning && !data_reading)
			state_frequence = 2;

		 if(right_motor_get_pos() == position_to_reach_r && left_motor_get_pos() == position_to_reach_l)
		 {
			 play_record = false;//a enlever
			 //data_reading = true;
			 turning = false;
		 }
		 */
	}

	last_state_frequence = state_frequence;

	/*--------------------*/
	//mémorise avancer/reculer/stop
	if(state_frequence == 0 || state_frequence == 3 || state_frequence == 4)
		last_direction = state_frequence;

	//continue la mouvement précédant(avancer/reculer/stop) après une rotation
	if(state_frequence == 5)
		state_frequence = last_direction;
/*
	if(state_frequence == 5)
		state_frequence = 4;
*/
	for(int i = 0; i < 8; i++){
		if(get_calibrated_prox(i) > 600)
			state_frequence = 4;
	}

	switch(state_frequence){
		case 0://forward
			//chprintf((BaseSequentialStream *)&SD3, "Forw\n");
			left_motor_set_speed(600);
			right_motor_set_speed(600);
			break;
		case 1://left
			//chprintf((BaseSequentialStream *)&SD3, "Left\n");
			left_motor_set_speed(-600);
			right_motor_set_speed(600);
			break;
		case 2://right
			//chprintf((BaseSequentialStream *)&SD3, "Right\n");
			left_motor_set_speed(600);
			right_motor_set_speed(-600);
			break;
		case 3://backward
			//chprintf((BaseSequentialStream *)&SD3, "Back\n");
			left_motor_set_speed(-600);
			right_motor_set_speed(-600);
			break;
		case 4://stop
			//chprintf((BaseSequentialStream *)&SD3, "Stop\n");
			left_motor_set_speed(0);
			right_motor_set_speed(0);
			break;
	}
}

bool toggle_record(void)
{
	static bool last_button_state = false;
	if(!button_get_state() && last_button_state)
	{
		last_button_state = button_get_state();
		return true;
	}

	last_button_state = button_get_state();
	return false;
}
/*
uint8_t filtre_freq(uint8_t now_state)
{
	//prev1_state <-> prev2_state <-> now_state
	static uint8_t prev1_state = 4;
	static uint8_t prev2_state = 4;
	if(prev2_state != now_state)
	{
		if(prev1_state == prev2_state)
		{
			prev1_state = prev2_state;
			prev2_state = now_state;
			return prev2_state;
		}
	}

	prev1_state = prev2_state;
	prev2_state = now_state;
	return now_state;
}
*/
