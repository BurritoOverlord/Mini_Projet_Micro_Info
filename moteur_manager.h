#ifndef MOTEUR_MANAGER_H
#define MOTEUR_MANAGER_H

void moteur_manager_deplacement(uint8_t state_frequence);

void robot_mvt(uint8_t state_frequence, int speed);

int toggle_speed(uint8_t state_frequence, bool recording, bool play_record, int speed);

bool toggle_record(void);

uint8_t reverse_state_frequence(uint8_t state_frequence);

#endif /*MOTEUR_MANAGER_H*/
