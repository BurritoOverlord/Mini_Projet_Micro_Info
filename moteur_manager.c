#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include <main.h>
#include <usbcfg.h>
#include <chprintf.h>

#include <motors.h>
#include <audio_processing.h>
#include <sensors/proximity.h>
#include <moteur_manager.h>
#include <animation_manager.h>
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
	if(toggle_record() || end_record){
		if (recording) {
			recording = false;
			play_record = true;
			end_record = false;
			stopCurrentMelody();
		} else {
			recording = true;
		}
	}

	//change speed
	speed = toggle_speed(state_frequence, recording, play_record, speed);

	/*****Contrainte Mouvement*****/
	if (state_frequence == 0 || state_frequence == 3 || state_frequence == 4)
		last_direction = state_frequence;	//mémorise avancer/reculer/stop

	if (state_frequence == 6 || state_frequence == 5)
		state_frequence = last_direction;	//continue la mouvement précédant(avancer/reculer/stop) après une rotation ou si changement de vitesse
	/*****Contrainte Mouvement*****/

	/******Collision detection*****/
	bool sens_num[8];
	bool coll_detected = false;

	for (int i = 0; i < 8; i++) {
		if (get_calibrated_prox(i) > 600) {
			sens_num[i] = true;
			coll_detected = true;
		} else
			sens_num[i] = false;
	}
	/******Collision detection*****/


	/*****Record et Play Record*****/
	//GAMEOVER
	if (recording && coll_detected) {
		end_record = true;
		gameover = true;
	}

	//RECORD
	if (recording) {
		if (rec_num < MAX_REC_TIME) {
			rec_data[rec_num] = state_frequence;
			rec_num++;
			if (rec_num == 714) //environ 10s avant fin record => son bombe
				play_external_melody();
		} else {
			end_record = true;
			gameover = true;
			stopCurrentMelody();
		}
	}

	//PLAY_RECORD
	if (play_record) {
		if (rec_num != 0) {
			rec_num--;
			state_frequence = reverse_state_frequence(rec_data[rec_num]);
		} else {
			play_record = false;
			gameover = false;
			rec_num = 0;
			state_frequence = last_direction = 4;
			stopCurrentMelody();
		}
	}
	/*****Record et Play Record*****/

	/*****Mouvement*****/
	robot_mvt(state_frequence, speed);
	/*****Mouvement*****/

	/*****Animation*****/
	animation(recording, play_record, coll_detected, gameover, sens_num);
	/*****Animation*****/
}

void robot_mvt(uint8_t state_frequence, int speed) {
	switch (state_frequence) {
	case 0: //forward
		left_motor_set_speed(speed);
		right_motor_set_speed(speed);
		break;
	case 1: //left
		left_motor_set_speed(-speed / 2);
		right_motor_set_speed(speed / 2);
		break;
	case 2: //right
		left_motor_set_speed(speed / 2);
		right_motor_set_speed(-speed / 2);
		break;
	case 3: //backward
		left_motor_set_speed(-speed);
		right_motor_set_speed(-speed);
		break;
	case 4: //stop
		left_motor_set_speed(0);
		right_motor_set_speed(0);
		break;
	}
}

int toggle_speed(uint8_t state_frequence, bool recording, bool play_record, int speed) {
	static uint8_t last_freq_state = 4;
	if (state_frequence == 5 && last_freq_state != 5 && !recording && !play_record) {
		last_freq_state = state_frequence;

		switch (speed) {
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

bool toggle_record(void) {
	static bool last_button_state = false;

	if (!button_get_state() && last_button_state) {
		last_button_state = button_get_state();
		return true;
	}

	last_button_state = button_get_state();
	return false;
}

uint8_t reverse_state_frequence(uint8_t state_frequence) {
	switch (state_frequence) {
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
