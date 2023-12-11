#include <stdio.h>
#include "stm32f10x.h"
#include <stdbool.h>
#include <sys/time.h>

void initial_Screen(void);
int set_Seed(void);
uint8_t *generate_Sequence(uint8_t *drawn_Sequence);
void display_Sequence(uint8_t *drawn_Sequence, uint16_t display_Time);
uint8_t *take_Movements(uint8_t *player_Sequence, uint16_t movement_Speed);
bool compare_Sequences(uint8_t *drawn_Sequence, uint8_t *player_Sequence);
void end_Game(uint16_t score);
void display_Score(uint16_t score);
void draw_Joystick(void);
