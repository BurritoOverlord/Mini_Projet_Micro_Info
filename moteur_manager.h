#ifndef MOTEUR_MANAGER_H
#define MOTEUR_MANAGER_H

void moteur_manager_deplacement(uint8_t state_frequence);

void robot_mvt(uint8_t state_frequence, int speed);

int toggle_speed(uint8_t state_frequence, int speed);

bool toggle_record(void);

uint8_t reverse_state_frequence(uint8_t state_frequence);

void animation_manager(bool recording, bool play_record, bool coll_detected, bool gameover, bool sens_num[]);

void on_off_all_LED(bool led1, bool led2, bool led3, bool led4, bool led5, bool led6, bool led7, bool led8, bool body_led, bool front_led);

#endif /*MOTEUR_MANAGER_H*/
