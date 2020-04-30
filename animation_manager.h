/*
 * animation_manager.h
 *
 *  Created on: 30 avr. 2020
 *      Author: armand
 */

#ifndef ANIMATION_MANAGER_H_
#define ANIMATION_MANAGER_H_

#define	REC_LED_ANIMATION	1,0,0,0,0,0,0,0,1,1,\
							1,0,0,0,0,0,0,0,1,1,\
							1,1,0,0,0,0,0,0,1,1,\
							1,1,1,0,0,0,0,0,1,1,\
							1,1,1,1,0,0,0,0,1,1,\
							1,1,1,1,1,0,0,0,1,1,\
							1,1,1,1,1,1,0,0,1,1,\
							1,1,1,1,1,1,1,0,1,1,\
							1,1,1,1,1,1,1,1,1,1,\
							1,1,1,1,1,1,1,1,1,1,\
							1,0,1,1,1,1,1,1,1,1,\
							1,0,0,1,1,1,1,1,1,1,\
							1,0,0,0,1,1,1,1,1,1,\
							1,0,0,0,0,1,1,1,1,1,\
							1,0,0,0,0,0,1,1,1,1,\
							1,0,0,0,0,0,0,1,1,1

#define	GAMEOVER_LED_ANIMATION	1,0,0,0,0,0,0,0,0,0,\
								1,0,0,0,0,0,0,0,0,0,\
								0,1,0,0,0,0,0,1,0,0,\
								0,1,0,0,0,0,0,1,0,0,\
								0,0,1,0,0,0,1,0,0,0,\
								0,0,1,0,0,0,1,0,0,0,\
								0,0,0,1,0,1,0,0,0,0,\
								0,0,0,1,0,1,0,0,0,0,\
								0,0,0,0,1,0,0,0,0,0,\
								0,0,0,0,1,0,0,0,0,0


void animation(bool recording, bool play_record, bool coll_detected, bool gameover, bool sens_num[]);

void on_off_all_LED(bool led1, bool led2, bool led3, bool led4, bool led5, bool led6, bool led7, bool led8, bool body_led, bool front_led);

void play_external_melody(void);

#endif /* ANIMATION_MANAGER_H_ */
