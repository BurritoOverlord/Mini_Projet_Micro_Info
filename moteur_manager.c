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
#include <audio/play_melody.h>

#define MAX_REC_TIME	857//70ms * 857 = 59.990s
#define HARDCORE_SPEED	MOTOR_SPEED_LIMIT
#define VETERAN_SPEED	800
#define NORMAL_SPEED	500


void moteur_manager_deplacement(uint8_t state_frequence)
{
	static uint8_t last_direction;
	static uint8_t rec_data[MAX_REC_TIME];
	static uint16_t rec_num = 0;
	static int speed = NORMAL_SPEED;
	static bool recording = false;
	static bool play_record = false;
	static bool end_record = false;
	static bool gameover = false;

	//Start game and start recording
	if(toggle_record() || end_record)
	{
		if(recording){
			recording = false;
			play_record = true;
			end_record = false;
		}
		else
		{
			recording = true;
		}
	}

	speed = toggle_speed(state_frequence, speed);

	/*****Contrainte Mouvement*****/
	//mémorise avancer/reculer/stop
	if(state_frequence == 0 || state_frequence == 3 || state_frequence == 4)
		last_direction = state_frequence;

	//continue la mouvement précédant(avancer/reculer/stop) après une rotation ou si changement de difficulté
	if(state_frequence == 6 || state_frequence == 5)
		state_frequence = last_direction;
	/*****Contrainte Mouvement*****/


	/******Collision detection*****/
	bool sens_num[8];
	bool coll_detected = false;

	for(int i = 0; i < 8; i++){
		if(get_calibrated_prox(i) > 600){
			sens_num[i] = true;
			coll_detected = true;
		}
		else
			sens_num[i] = false;
	}
	/******Collision detection*****/


	/*****Record et Play Record*****/
	//GAMEOVER
	if(recording && coll_detected){
		end_record = true;
		gameover = true;
		//game_over_animation(true);
	}

	//RECORD
	if(recording){
		if(rec_num < MAX_REC_TIME){
			rec_data[rec_num] = state_frequence;
			rec_num++;
		}
		else{
			end_record = true;
			gameover = true;
		}
	}

	//PLAY_RECORD
	if(play_record){
		if(rec_num != 0)
		{
			rec_num--;
			state_frequence = reverse_state_frequence(rec_data[rec_num]);
		}
		else
		{
			play_record = false;
			gameover = false;
			rec_num = 0;
			state_frequence = 4;
		}
	}
	/*****Record et Play Record*****/


	/*****Mouvement*****/
	robot_mvt(state_frequence, speed);
	/*****Mouvement*****/


	/*****Animation*****/
	animation_manager(recording, play_record, coll_detected, gameover, sens_num);
	/*****Animation*****/
}

void robot_mvt(uint8_t state_frequence, int speed)
{
	switch(state_frequence){
		case 0://forward
			left_motor_set_speed(speed);
			right_motor_set_speed(speed);
			break;
		case 1://left
			left_motor_set_speed(-speed/2);
			right_motor_set_speed(speed/2);
			break;
		case 2://right
			left_motor_set_speed(speed/2);
			right_motor_set_speed(-speed/2);
			break;
		case 3://backward
			left_motor_set_speed(-speed);
			right_motor_set_speed(-speed);
			break;
		case 4://stop
			left_motor_set_speed(0);
			right_motor_set_speed(0);
			break;
	}
}

int toggle_speed(uint8_t state_frequence, int speed)
{
	static uint8_t last_freq_state = 4;
	if(state_frequence == 5 && last_freq_state != 5)
	{
		last_freq_state = state_frequence;
		switch(speed){
			case NORMAL_SPEED:
				return VETERAN_SPEED;
				break;
			case VETERAN_SPEED:
				return HARDCORE_SPEED;
				break;
			case HARDCORE_SPEED:
				return NORMAL_SPEED;
				break;
		}
	}

	last_freq_state = state_frequence;
	return speed;
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

uint8_t reverse_state_frequence(uint8_t state_frequence)
{
	switch(state_frequence){
		case 0:
			return 3;
			break;
		case 1:
			return 2;
			break;
		case 2:
			return 1;
			break;
		case 3:
			return 0;
			break;
		case 4:
			return 4;
			break;
	}
	return 4;

}

void animation_manager(bool recording, bool play_record, bool coll_detected, bool gameover, bool sens_num[])
{
	if(!recording && !play_record && coll_detected){//if collision
		on_off_all_LED(sens_num[0],sens_num[1],sens_num[2],sens_num[3],sens_num[4],sens_num[5],sens_num[6],sens_num[7],0,0);
	}
	else if(recording && !play_record){//recording
		on_off_all_LED(0,0,0,0,0,0,0,0,1,1);
	}
	else if(!recording && play_record && gameover){//Gameover
		on_off_all_LED(1,1,1,1,1,1,1,1,0,0);
		playMelody(MARIO_DEATH, ML_SIMPLE_PLAY, NULL);
	}
	else if(!recording && play_record && !gameover){//if Victory
		on_off_all_LED(1,1,1,1,1,1,1,1,1,1);
		playMelody(MARIO_FLAG, ML_SIMPLE_PLAY, NULL);
	}
	else//if nothing
		on_off_all_LED(0,0,0,0,0,0,0,0,0,0);
}

void on_off_all_LED(bool led1, bool led2, bool led3, bool led4, bool led5, bool led6, bool led7, bool led8, bool body_led, bool front_led)
{
	set_led(LED1, led1);
	set_rgb_led(LED2, led2 * 100, 0, 0);
	set_led(LED3, led3);
	set_rgb_led(LED4, led4 * 100, 0, 0);
	set_led(LED5, led5);
	set_rgb_led(LED6, led6 * 100, 0, 0);
	set_led(LED7, led7);
	set_rgb_led(LED8, led8 * 100, 0, 0);
	set_body_led(body_led);
	set_front_led(front_led);

}
