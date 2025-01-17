#ifndef MOTEUR_MANAGER_H
#define MOTEUR_MANAGER_H

/*
 * S'occupe de la prise de d�cision du robot. Le robot a 4 �tats possible:
 * 1.	"Normal", le robot est dirig� par les fr�quences
 * 2.	record, le robot est dirig� comme en normal, mais enregistre tout ses d�placements
 * 3.	game_over, c'est perdu, quand le robot entre en collision avec un mur et qu'il enregistrait. Le robot retourne sur ses pas.
 * 4.	play_record, c'est gagn�, record sans collision. Le robot retourne sur ses pas.
 *
 * param:
 * uint8_t	state_frequence		Re�oit la fr�quence processer par "processAudioData" et trier par "sound_remote".
 */
void moteur_manager_deplacement(uint8_t state_frequence);

/*
 * D�placement du robot en fonction du state_frequence re�u
 *
 * param:
 * uint8_t 	state_frequence		mouvement a ex�cuter
 * int 		speed				vitesse de d�placement
 */
void robot_mvt(uint8_t state_frequence, int speed);

/*
 * Retourne la prochaine vitesse, lorsque la frequence correspondante est re�u (state_frequence = 5)
 * Ordre des vitesse: NORMAL_SPEED -> VETERAN_SPEED -> HARDCORE_SPEED -> NORMAL_SPEED ->...
 *
 * param:
 * uint8_t 	state_frequence		fr�quence re�u
 * bool 	recording			est-il en mode enregistrement ?
 * bool 	play_record			est-il en mode play_record ?
 * int 		speed				vitesse actuelle de d�placement du robot
 */
int toggle_speed(uint8_t state_frequence, bool recording, bool play_record, int speed);

//retourne vrai quand le bouton "USER" termine d'�tre appuyer
bool toggle_record(void);

/*
 * Permet de faire les mouvements inverse que la fonction robot_mvt() lorsque play_record et gameover
 *
 * param:
 * uint8_t 	state_frequence		fr�quence qui doit �tre invers�
 */
uint8_t reverse_state_frequence(uint8_t state_frequence);

#endif /*MOTEUR_MANAGER_H*/
