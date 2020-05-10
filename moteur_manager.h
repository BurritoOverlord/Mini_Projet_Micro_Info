#ifndef MOTEUR_MANAGER_H
#define MOTEUR_MANAGER_H

/*
 * S'occupe de la prise de décision du robot. Le robot a 4 êtats possible:
 * 1.	"Normal", le robot est dirigé par les fréquences
 * 2.	record, le robot est dirigé comme en normal, mais enregistre tout ses déplacements
 * 3.	game_over, c'est perdu, quand le robot entre en collision avec un mur et qu'il enregistrait. Le robot retourne sur ses pas.
 * 4.	play_record, c'est gagné, record sans collision. Le robot retourne sur ses pas.
 *
 * param:
 * uint8_t	state_frequence		Reçoit la fréquence processer par "processAudioData" et trier par "sound_remote".
 */
void moteur_manager_deplacement(uint8_t state_frequence);

/*
 * Déplacement du robot en fonction du state_frequence reçu
 *
 * param:
 * uint8_t 	state_frequence		mouvement a exécuter
 * int 		speed				vitesse de déplacement
 */
void robot_mvt(uint8_t state_frequence, int speed);

/*
 * Retourne la prochaine vitesse, lorsque la frequence correspondante est reçu (state_frequence = 5)
 * Ordre des vitesse: NORMAL_SPEED -> VETERAN_SPEED -> HARDCORE_SPEED -> NORMAL_SPEED ->...
 *
 * param:
 * uint8_t 	state_frequence		fréquence reçu
 * bool 	recording			est-il en mode enregistrement ?
 * bool 	play_record			est-il en mode play_record ?
 * int 		speed				vitesse actuelle de déplacement du robot
 */
int toggle_speed(uint8_t state_frequence, bool recording, bool play_record, int speed);

//retourne vrai quand le bouton "USER" termine d'être appuyer
bool toggle_record(void);

/*
 * Permet de faire les mouvements inverse que la fonction robot_mvt() lorsque play_record et gameover
 *
 * param:
 * uint8_t 	state_frequence		fréquence qui doit être inversé
 */
uint8_t reverse_state_frequence(uint8_t state_frequence);

#endif /*MOTEUR_MANAGER_H*/
