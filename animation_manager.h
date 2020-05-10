#ifndef ANIMATION_MANAGER_H_
#define ANIMATION_MANAGER_H_

#define	REC_LED_ANIMATION	1,0,0,0,0,0,0,0,1,0,\
							1,0,0,0,0,0,0,0,1,0,\
							1,1,0,0,0,0,0,0,1,0,\
							1,1,1,0,0,0,0,0,1,0,\
							1,1,1,1,0,0,0,0,1,0,\
							1,1,1,1,1,0,0,0,1,0,\
							1,1,1,1,1,1,0,0,1,0,\
							1,1,1,1,1,1,1,0,1,0,\
							1,1,1,1,1,1,1,1,1,0,\
							1,1,1,1,1,1,1,1,1,0,\
							1,0,1,1,1,1,1,1,1,0,\
							1,0,0,1,1,1,1,1,1,0,\
							1,0,0,0,1,1,1,1,1,0,\
							1,0,0,0,0,1,1,1,1,0,\
							1,0,0,0,0,0,1,1,1,0,\
							1,0,0,0,0,0,0,1,1,0

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

#define	WIN_LED_ANIMATION	1,0,0,0,0,0,0,0,0,0,\
							0,1,0,0,0,0,0,0,0,0,\
							0,0,1,0,0,0,0,0,0,0,\
							0,0,0,1,0,0,0,0,0,0,\
							0,0,0,0,1,0,0,0,0,0,\
							0,0,0,0,0,1,0,0,0,0,\
							0,0,0,0,0,0,1,0,0,0,\
							0,0,0,0,0,0,0,1,0,0,\
							1,1,1,1,1,1,1,1,1,0,\
							1,1,1,1,1,1,1,1,1,0,\
							0,0,0,0,0,0,0,0,0,0,\
							0,0,0,0,0,0,0,0,0,0,\
							1,1,1,1,1,1,1,1,1,0,\
							1,1,1,1,1,1,1,1,1,0,\
							0,0,0,0,0,0,0,0,0,0,\
							0,0,0,0,0,0,0,0,0,0,\
							0,0,0,0,0,0,0,1,0,0,\
							0,0,0,0,0,0,1,0,0,0,\
							0,0,0,0,0,1,0,0,0,0,\
							0,0,0,0,1,0,0,0,0,0,\
							0,0,0,1,0,0,0,0,0,0,\
							0,0,1,0,0,0,0,0,0,0,\
							0,1,0,0,0,0,0,0,0,0,\
							1,0,0,0,0,0,0,0,0,0,\
							1,1,1,1,1,1,1,1,1,0,\
							1,1,1,1,1,1,1,1,1,0,\
							0,0,0,0,0,0,0,0,0,0,\
							0,0,0,0,0,0,0,0,0,0,\
							1,1,1,1,1,1,1,1,1,0,\
							1,1,1,1,1,1,1,1,1,0,\
							0,0,0,0,0,0,0,0,0,0,\
							0,0,0,0,0,0,0,0,0,0

/*
 * Fonction principale pour la gestion des animations du robot (LED et musique)
 *
 * param:
 * bool 	recording		le robot, est-il en mode enregistrement ?
 * bool 	play_record		est-il en mode play_record ?
 * bool 	coll_detected	y a-t-il eu une collision ?
 * bool		gameover		est-il en mode gameover ?
 * bool 	sens_num[]		tableau des senseurs de distance qui ont détecté une collision (1=collision, 0=pas collision)
 */
void animation(bool recording, bool play_record, bool coll_detected, bool gameover, bool sens_num[]);

/*
 * Allume ou éteint chaque LED une par une. (1=allumé, 0=éteint)
 *
 * param:
 * bool ledx	êtat voulu de le LED (x=1,2,3,4,5,6,7,8 ou body_led ou front_led)
 */
void on_off_all_LED(bool led1, bool led2, bool led3, bool led4, bool led5, bool led6, bool led7, bool led8, bool body_led, bool front_led);

/*
 * Joue la mélody de la bombe
 */
void play_external_melody(void);

#endif /* ANIMATION_MANAGER_H_ */
