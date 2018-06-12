#pragma once

#ifndef _LIGHT_JUMPER_DEFINES
#define _LIGHT_JUMPER_DEFINES

// APP -------------------------------------------------------------------
#define APP_WIDTH                       711
#define APP_HEIGT                       768
#define APP_WIDTH_MEITAT                355
#define APP_HEIGT_MEITAT                384

#define NUM_ROWS                        9 // GRID
#define NUM_COLS                        9

#define MAX_NUM_BLOBS                   10 // DETECCIÓ

#define MAX_GAME_TIME                   0.5 // JOC
#define NUM_PANTALLES                   2
#define NUM_PECES_MIN                   180
#define NUM_PECES_TOTAL                 (MAX_GAME_TIME * NUM_PECES_MIN) - 10


#define IDLE_TIMER_PECA                 240
#define TOCADA_TIMER_PECA               30

enum pantallesJoc{
    START,
    PLAY,
    END,
    HIGH_SCORES,
    TUTORIAL_1,
    TUTORIAL_2,
    TUTORIAL_3
};

// http://www.cplusplus.com/forum/beginner/41790/
// Overload the pantallesJoc++ operator
inline pantallesJoc& operator++(pantallesJoc& eDOW, int){
	const int i = static_cast<int>(eDOW)+1;
	eDOW = static_cast<pantallesJoc>((i) % NUM_PANTALLES);
	return eDOW;
}

// Overload the pantallesJoc-- operator
inline pantallesJoc& operator--(pantallesJoc& eDOW, int){
	const int i = static_cast<int>(eDOW)-1;
	if (i < 0){ // Check whether to cycle to last item if number goes below 0
		eDOW = static_cast<pantallesJoc>(NUM_PANTALLES);
	}
	else{ // Else set it to current number -1
		eDOW = static_cast<pantallesJoc>((i) % NUM_PANTALLES);
	}
	return eDOW;
}

#endif
