/*
 * animation_manager.c
 *
 *  Created on: 30 avr. 2020
 *      Author: armand
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include <main.h>
#include <chprintf.h>

#include <animation_manager.h>
#include <spi_comm.h>
#include <leds.h>
#include <audio/play_melody.h>

//Bomb melody
static const uint16_t bomb_melody[] =
{
	    NOTE_C7,0,0,0,0,0,0,0,0,0,0,0,
		NOTE_C7,0,0,0,0,0,0,0,0,0,0,
		NOTE_C7,0,0,0,0,0,0,0,0,0,
		NOTE_C7,0,0,0,0,0,0,0,0,
		NOTE_C7,0,0,0,0,0,0,
		NOTE_C7,0,0,0,0,0,
		NOTE_C7,0,0,0,0,
		NOTE_C7,0,0,0,
		NOTE_C7,0,NOTE_C7,
		0,NOTE_C7,
		NOTE_C7
};

//Bomb tempo
static const float bomb_tempo[] =
{
    20,20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,20,
    20,20,20,20,20,20,
    20,20,20,20,20,
    20,20,20,20,
    20,20,20,
	20,20,
	1
};

static const melody_t melody[1] = {

		  //EXTERNAL_SONG
		  {
		    .notes = bomb_melody,
		    .tempo = bomb_tempo,
		    .length = sizeof(bomb_melody)/sizeof(uint16_t),
		  },
};

void animation(bool recording, bool play_record, bool coll_detected, bool gameover, bool sens_num[])
{
	static uint8_t memo_i_record = 0;
	static uint8_t memo_i_gameover = 0;

	if(!recording && !play_record && coll_detected){//if collision
		on_off_all_LED(sens_num[0],sens_num[1],sens_num[2],sens_num[3],sens_num[4],sens_num[5],sens_num[6],sens_num[7],0,0);
	}
	else if(recording && !play_record){//recording
			bool tab[160] = {REC_LED_ANIMATION};
			uint8_t i = memo_i_gameover;
			on_off_all_LED(tab[i],tab[i+1],tab[i+2],tab[i+3],tab[i+4],tab[i+5],tab[i+6],tab[i+7],tab[i+8],tab[i+9]);
			memo_i_gameover += 10;
			if(memo_i_gameover == 160)
				memo_i_gameover = 0;
	}
	else if(!recording && play_record && gameover){//Gameover
		bool tab[100] = {GAMEOVER_LED_ANIMATION};
		uint8_t i = memo_i_record;
		on_off_all_LED(tab[i],tab[i+1],tab[i+2],tab[i+3],tab[i+4],tab[i+5],tab[i+6],tab[i+7],tab[i+8],tab[i+9]);
		memo_i_record += 10;
		if(memo_i_record == 100)
			memo_i_record = 0;
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

void play_external_melody(void)
{
	playMelody(EXTERNAL_SONG, ML_SIMPLE_PLAY, melody);
}

