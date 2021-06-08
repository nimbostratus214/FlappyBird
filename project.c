 
///////////////////////////////////////////////////////////////////////////////
// Headers.

#include <stdint.h>
#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "spritesFB_rgb333.h"

///////////////////////////////////////////////////////////////////////////////
// HW stuff.

#define WAIT_UNITL_0(x) while(x != 0){}
#define WAIT_UNITL_1(x) while(x != 1){}

#define SCREEN_IDX1_W 640
#define SCREEN_IDX1_H 480
#define SCREEN_IDX4_W 320
#define SCREEN_IDX4_H 240
#define SCREEN_RGB333_W 160
#define SCREEN_RGB333_H 120

#define SCREEN_IDX4_W8 (SCREEN_IDX4_W/8)

#define gpu_p32 ((volatile uint32_t*)LPRS2_GPU_BASE)
#define palette_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x1000))
#define unpack_idx1_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x400000))
#define pack_idx1_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x600000))
#define unpack_idx4_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x800000))
#define pack_idx4_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0xa00000))
#define unpack_rgb333_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0xc00000))
#define joypad_p32 ((volatile uint32_t*)LPRS2_JOYPAD_BASE)

typedef struct {
	unsigned a      : 1;
	unsigned b      : 1;
	unsigned z      : 1;
	unsigned start  : 1;
	unsigned up     : 1;
	unsigned down   : 1;
	unsigned left   : 1;
	unsigned right  : 1;
} bf_joypad;
#define joypad (*((volatile bf_joypad*)LPRS2_JOYPAD_BASE))

typedef struct {
	uint32_t m[SCREEN_IDX1_H][SCREEN_IDX1_W];
} bf_unpack_idx1;
#define unpack_idx1 (*((volatile bf_unpack_idx1*)unpack_idx1_p32))



///////////////////////////////////////////////////////////////////////////////
// Game config.

//#define STEP 1
#define STEP_BIRD 1


#define background_ANIM_DELAY 1
#define BIRD_ANIM_DELAY 3



///////////////////////////////////////////////////////////////////////////////
// Game data structures.

typedef struct {
	uint16_t x;
	uint16_t y;
} point_t;



typedef enum {
	background_IDLE,
	background_GAME,
	background_GAME_OVER
} background_anim_states_t;

//tica
typedef enum {
	BIRD_IDLE,
	BIRD_OPENING_WING_UP,
	BIRD_WING_UP,
	BIRD_WING_MIDDLE,
	BIRD_WING_DOWN
} bird_anim_states_t;

//cevcuga
typedef enum {
	TUBE_BEGIN,
	TUBE_IDLE,
	TUBE_GO
} tube_anim_states_t;

//skor
typedef enum {
	NUM_ZERO,
	NUM_ONE,
	NUM_TWO,
	NUM_THREE,
	NUM_FOUR,
	NUM_FIVE,
	NUM_SIX,
	NUM_SEVEN,
	NUM_EIGHT,
	NUM_NINE,
	NONE
} score_anim_states_t;

///////////////////////

typedef struct {
	background_anim_states_t state;
	uint8_t delay_cnt;
} background_anim_t;

//tica
typedef struct {
	bird_anim_states_t birdState;
	uint8_t brojac;
}bird_anim_t;

//cevcuga//
typedef struct {
	tube_anim_states_t tubeState;
	uint8_t brojac;
}tube_anim_t;

//skor
typedef struct {
	score_anim_states_t scoreState;
	uint8_t brojac;
}score_anim_t;

///////////////////////

typedef struct {
	point_t pos;
	background_anim_t anim;
} background_t;

//tica
typedef struct {
	point_t pozicija;
	bird_anim_t animBird;
} bird_t;

//cevcuga
typedef struct {
	point_t pozicija;
	tube_anim_t tube_anim;
} tube_t;

//skor
typedef struct {
	point_t pozicija;
	score_anim_t animScore;
}score_t;

///////////////////////

typedef struct {
	background_t background;
} game_state_t;

//tica
typedef struct {
	bird_t bird;
} game_bird_state_t;

//cevcuga//
typedef struct {
	tube_t tube;
	int has_scored;
} game_tube_state_t;

//skor
typedef struct {
	score_t score;
} game_score_state_t;

///////////////////////





void draw_sprite_from_atlas(
	uint16_t src_x,
	uint16_t src_y,
	uint16_t w,
	uint16_t h,
	uint16_t dst_x,
	uint16_t dst_y
) {
	
	
	for(uint16_t y = 0; y < h; y++){
		for(uint16_t x = 0; x < w; x++){
			uint32_t src_idx = 
				(src_y+y)*FB_Sprite_Map__w +
				(src_x+x);
			uint32_t dst_idx = 
				(dst_y+y)*SCREEN_RGB333_W +
				(dst_x+x);
			uint16_t pixel = FB_Sprite_Map__p[src_idx];
			unpack_rgb333_p32[dst_idx] = pixel;
		}
	}
	
	
}


void drawNumbers(game_score_state_t num_state) 
{
	switch(num_state.score.animScore.scoreState) {
			case NUM_ZERO:
				draw_sprite_from_atlas(
					286, 97, 12, 18, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_ONE:
				draw_sprite_from_atlas(
					270, 79, 8, 18, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_TWO:
				draw_sprite_from_atlas(
					357, 147, 12, 18, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_THREE:
				draw_sprite_from_atlas(
					371, 147, 12, 18, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;	
			case NUM_FOUR:
				draw_sprite_from_atlas(
					385, 147, 12, 18, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_FIVE:
				draw_sprite_from_atlas(
					399, 147, 12, 18, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_SIX:
				draw_sprite_from_atlas(
					357, 171, 12, 18, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_SEVEN:
				draw_sprite_from_atlas(
					371, 171, 12, 18, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_EIGHT:
				draw_sprite_from_atlas(
					385, 171, 12, 18, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_NINE:
				draw_sprite_from_atlas(
					399, 171, 12, 18, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NONE:
				draw_sprite_from_atlas(
					260, 30, 12, 18, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
		}
}


void drawNumbersSmall(game_score_state_t num_state) 
{
	switch(num_state.score.animScore.scoreState) {
			case NUM_ZERO:
				draw_sprite_from_atlas(
					320, 125, 7, 10, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_ONE:
				draw_sprite_from_atlas(
					273, 101, 5, 10, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_TWO:
				draw_sprite_from_atlas(
					271, 113, 7, 10, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_THREE:
				draw_sprite_from_atlas(
					271, 125, 7, 10, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;	
			case NUM_FOUR:
				draw_sprite_from_atlas(
					341, 97, 7, 10, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_FIVE:
				draw_sprite_from_atlas(
					341, 109, 7, 10, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_SIX:
				draw_sprite_from_atlas(
					281, 125, 7, 10, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_SEVEN:
				draw_sprite_from_atlas(
					291, 125, 7, 10, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_EIGHT:
				draw_sprite_from_atlas(
					301, 125, 7, 10, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NUM_NINE:
				draw_sprite_from_atlas(
					310, 125, 7, 10, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
				break;
			case NONE:
				draw_sprite_from_atlas(
					372, 210, 6, 7, num_state.score.pozicija.x, num_state.score.pozicija.y
				);
		}
}

///////////////////////////////////////////////////////////////////////////////
// Game code.

volatile int best_score = 0;

int main(void) {

	srand(time(0));
	
	int STEP = 1;
	
	// Setup.
	gpu_p32[0] = 3; // RGB333 mode.
	gpu_p32[0x800] = 0x00ff00ff; // Magenta for HUD.


	// Game state.
	game_state_t gs;
	gs.background.pos.x = 0;
	gs.background.pos.y = 0;
	gs.background.anim.state = background_IDLE;
	gs.background.anim.delay_cnt = 0;
	
	
	//bird
	game_bird_state_t gbs;
	gbs.bird.pozicija.x = 20;
	gbs.bird.pozicija.y = 40;
	gbs.bird.animBird.birdState = BIRD_IDLE;
	gbs.bird.animBird.brojac = 0;

	//tube
	game_tube_state_t gts;
	gts.tube.pozicija.x = 100; // move right
	gts.tube.pozicija.y = 71;
	gts.tube.tube_anim.tubeState = TUBE_BEGIN;
	gts.tube.tube_anim.brojac = 0;
	gts.has_scored = 0;

	//another tube 2
	game_tube_state_t gts2;
	gts2.tube.pozicija.x = 110;
	gts2.tube.pozicija.y = 71;
	gts2.tube.tube_anim.tubeState = TUBE_BEGIN;
	gts2.tube.tube_anim.brojac = 0;
	gts2.has_scored = 0;

	//score_jedinice
	game_score_state_t gss1;
	gss1.score.pozicija.x = 78;
	gss1.score.pozicija.y = 100;
	gss1.score.animScore.scoreState = NUM_ZERO;
	gss1.score.animScore.brojac = 0;
	
	//score_desetice
	game_score_state_t gss2;
	gss2.score.pozicija.x = 64;
	gss2.score.pozicija.y = 100;
	gss2.score.animScore.scoreState = NONE;
	gss2.score.animScore.brojac = 0;

	int mov_x = 0;
	int bio = 0;
	
	int poginulaTica = 0;
	int score = 0;

	int inGame = 1;
	
	while(inGame){
		
		/////////////////////////////////////
		// Poll controls.
		
		int mov_y = 0;
		
		int movB_y = 0;
		int movB_x = 0;
			
		if(joypad.right && gs.background.anim.state == background_IDLE) {
			gs.background.anim.state = background_GAME;
			mov_x = -1;
		}
		
		if(joypad.up && gs.background.anim.state == background_GAME) {
			movB_y = -1;
		}
		if(joypad.down && gs.background.anim.state == background_GAME) {
			movB_y = +1;
		}
		
		if (joypad.a && gs.background.anim.state == background_GAME_OVER) {
			
			
			gs.background.pos.x = 0;
			gs.background.pos.y = 0;
			gs.background.anim.state = background_IDLE;
			gs.background.anim.delay_cnt = 0;
			
			gbs.bird.pozicija.x = 20;
			gbs.bird.pozicija.y = 40;
			gbs.bird.animBird.birdState = BIRD_IDLE;
			gbs.bird.animBird.brojac = 0;
			
			gts.tube.pozicija.x = 100; // move right
			gts.tube.pozicija.y = 71;
			gts.tube.tube_anim.tubeState = TUBE_BEGIN;
			gts.tube.tube_anim.brojac = 0;
			gts.has_scored = 0;
			
			gts2.tube.pozicija.x = 110;
			gts2.tube.pozicija.y = 71;
			gts2.tube.tube_anim.tubeState = TUBE_BEGIN;
			gts2.tube.tube_anim.brojac = 0;
			gts2.has_scored = 0;
			
			gts2.tube.pozicija.x = 110;
			gts2.tube.pozicija.y = 71;
			gts2.tube.tube_anim.tubeState = TUBE_BEGIN;
			gts2.tube.tube_anim.brojac = 0;
			gts2.has_scored = 0;
			
			gss2.score.pozicija.x = 64;
			gss2.score.pozicija.y = 100;
			gss2.score.animScore.scoreState = NONE;
			gss2.score.animScore.brojac = 0;
			mov_x = 0;
			bio = 0;
			poginulaTica = 0;
			score = 0;
			inGame = 1;
			STEP = 1;
			
		}
		
		//end game-izlazi iz while
		if (joypad.left && gs.background.anim.state == background_GAME_OVER) {
			//gs.background.anim.state = background_IDLE;	
			
			
			//mov_x = -1;
			inGame = 0;
			
		}
		/////////////////////////////////////
		// Gameplay.
		
		gs.background.pos.x += mov_x*STEP;		
		
		gbs.bird.pozicija.y += movB_y * STEP_BIRD; //tica ide gore-dole
		gbs.bird.pozicija.x += movB_x * STEP_BIRD;
		
		
		if(gs.background.pos.x < SCREEN_RGB333_W) {
			gs.background.pos.x = 2 * SCREEN_RGB333_W;
		}
		
		if(gbs.bird.pozicija.y > SCREEN_RGB333_H - 43) {
			gbs.bird.pozicija.y -= movB_y * STEP_BIRD;
		}
		
		if (score > best_score) {
			best_score = score;
		}
		
		
		switch(gbs.bird.animBird.birdState) {
			case BIRD_IDLE:
				if(movB_x != 0 || movB_y != 0) {
					gbs.bird.animBird.brojac = BIRD_ANIM_DELAY;
					gbs.bird.animBird.birdState = BIRD_WING_UP;
				}
				break;
			case BIRD_OPENING_WING_UP:
				if(gbs.bird.animBird.brojac != 0) {
					gbs.bird.animBird.brojac--;
				} else {
					gbs.bird.animBird.brojac = BIRD_ANIM_DELAY;
					gbs.bird.animBird.birdState = BIRD_WING_UP;
				}
				break;
			
			case BIRD_WING_UP:
				if(gbs.bird.animBird.brojac != 0) {
					gbs.bird.animBird.brojac--;
				} else {
					if(movB_x != 0 || movB_y != 0) {
						gbs.bird.animBird.brojac = BIRD_ANIM_DELAY;
						gbs.bird.animBird.birdState = BIRD_WING_MIDDLE;
					} else {
						gbs.bird.animBird.birdState = BIRD_IDLE;
					}
				}
				break;
				
			case BIRD_WING_MIDDLE:
				if(gbs.bird.animBird.brojac != 0) {
					gbs.bird.animBird.brojac--;
				} else {
					gbs.bird.animBird.brojac = BIRD_ANIM_DELAY;
					gbs.bird.animBird.birdState = BIRD_WING_DOWN;
				}
				break;
			case BIRD_WING_DOWN:
				if(gbs.bird.animBird.brojac != 0) {
					gbs.bird.animBird.brojac--;
				} else {
					if(movB_x != 0 || movB_y != 0) {
						gbs.bird.animBird.brojac = BIRD_ANIM_DELAY;
						gbs.bird.animBird.birdState = BIRD_OPENING_WING_UP;
					} else {
						gbs.bird.animBird.birdState = BIRD_IDLE;
					}
				}
				break;
		}

		float rnd;
		float rnd_par;

		// tubes first pair fsm
		switch(gts.tube.tube_anim.tubeState) {

			case TUBE_BEGIN:
				if(joypad.right) {
					bio++;
					rnd = rand()%35 + 1;	
					rnd_par = ((1-(rnd/35.0))*35.0);
					gts.has_scored = 0;
					gts.tube.tube_anim.tubeState = TUBE_GO;
				}
				else if(bio) {
					rnd = rand()%35 + 1;	
					rnd_par = ((1-(rnd/35.0))*35.0);
					gts.has_scored = 0;					
					gts.tube.tube_anim.tubeState = TUBE_GO;
				}
				break;

			case TUBE_GO:
				if(gts.tube.pozicija.x < 1) {
					gts.tube.pozicija.x = SCREEN_RGB333_W - 26; //work!
					gts.has_scored = 0;
					gts.tube.tube_anim.tubeState = TUBE_BEGIN;
				}
				gts.tube.pozicija.x -= STEP;
				break;
		}
				
		//check colision

        if (gbs.bird.pozicija.x >= gts.tube.pozicija.x - 13  && 
                gbs.bird.pozicija.x < gts.tube.pozicija.x + 22) {
					// y tice + visina tice >= y donje cevi ||
					// y tice <= y donje cevi - sirina prolaza
                    if(gbs.bird.pozicija.y + 12 >= 88 - (17+rnd) || 
                            gbs.bird.pozicija.y <= 88 - (17+rnd) - 20) {

                        gts.has_scored = 1;

                        poginulaTica++;
                        
                        if(poginulaTica >= 1) {
                            printf("KO VISOKO LETI, NISKO PADA!\n");
                            printf("Rezultat: %d\n", score);
                            STEP = 0;
							
                            gs.background.anim.state = background_GAME_OVER;
                        }
                        //return -1;
                    } 
					else if (gbs.bird.pozicija.x > gts.tube.pozicija.x - 17
                                    && gbs.bird.pozicija.x < gts.tube.pozicija.x + 26 && gts.has_scored == 0) {
                        gts.has_scored = 1;
                        score++;
                        if(score > 99) {
                            printf("Subotic je Car :smile: \n");
                            score = 0;
                            STEP = 0;
                            gs.background.anim.state = background_GAME_OVER;
                        }
                    }
                }
		
		float rnd2;
		float rnd_par2;
		
		if (score >= 5) {
			
				// tubes seccond pair fsm
			switch(gts2.tube.tube_anim.tubeState) {

				case TUBE_BEGIN:
					if(joypad.right) {
						bio++;
						rnd2 = rand()%35 + 1;	
						rnd_par2 = ((1-(rnd2/35.0))*35.0);
						gts2.has_scored = 0;
						gts2.tube.tube_anim.tubeState = TUBE_GO;
					}
					else if(bio) {
						rnd2 = rand()%35 + 1;	
						rnd_par2 = ((1-(rnd2/35.0))*35.0);
						gts2.has_scored = 0;					
						gts2.tube.tube_anim.tubeState = TUBE_GO;
					}
					break;

				case TUBE_GO:
					if(gts2.tube.pozicija.x < 1) {
						gts2.tube.pozicija.x = SCREEN_RGB333_W - 26; //work!
						gts2.has_scored = 0;
						gts2.tube.tube_anim.tubeState = TUBE_BEGIN;
					}
					gts2.tube.pozicija.x -= STEP;
					break;
			}
			
			//check colision 
			
			 if (gbs.bird.pozicija.x >= gts2.tube.pozicija.x - 13  && 
                gbs.bird.pozicija.x < gts2.tube.pozicija.x + 22) {
					// y tice + visina tice >= y donje cevi ||
					// y tice <= y donje cevi - sirina prolaza
                    if(gbs.bird.pozicija.y + 12 >= 88 - (17+rnd2) || 
                            gbs.bird.pozicija.y <= 88 - (17+rnd2) - 20) {

                        gts2.has_scored = 1;

                        poginulaTica++;
                        
                        if(poginulaTica >= 1) {
                            printf("KO VISOKO LETI, NISKO PADA!\n");
                            printf("Rezultat: %d\n", score);
                            STEP = 0;
                            gs.background.anim.state = background_GAME_OVER;
                        }
                        //return -1;
                    } 
					else if (gbs.bird.pozicija.x > gts2.tube.pozicija.x - 17
                                    && gbs.bird.pozicija.x < gts2.tube.pozicija.x + 26 && gts2.has_scored == 0) {
                        gts2.has_scored = 1;
                        score++;
                        if(score > 99) {
                            printf("Subotic je Car :smile: \n");
                            score = 0;
                            STEP = 0;
                            gs.background.anim.state = background_GAME_OVER;
                        }
                    }
                }
			
			
			
				
		}	
				
				
				
		//printf("y_pozicija tice %d :\n", gts.tube.pozicija.y);
		//printf("tube poz %d \n", gts.tube.pozicija.x); //pozicija ptice na x osi je uvek 20
		//do kolizije dolazi samo kada je i pozicija tube = 20
		
		
		//score
		int jed = 0;
		int des = 0;
		jed = score % 10;
		des = score / 10;
		if (des == 0)
			des = 10;
		gss2.score.animScore.scoreState = des;
		gss1.score.animScore.scoreState = jed;
		
		/////////////////////////////////////
		// Drawing.
		
		
		// Detecting rising edge of VSync.
		WAIT_UNITL_0(gpu_p32[2]);
		WAIT_UNITL_1(gpu_p32[2]);
		// Draw in buffer while it is in VSync.
		
		//That was a problem !
		
		// Black background.
		//for(uint16_t r = 0; r < SCREEN_RGB333_H; r++){
		//	for(uint16_t c = 0; c < SCREEN_RGB333_W; c++){
		//		unpack_rgb333_p32[r*SCREEN_RGB333_W + c] = 0000;
		//	}
		//}	
		
		//iscrtaj pozadinu
		switch(gs.background.anim.state){
			case background_IDLE:
				draw_sprite_from_atlas(
					0, 100, 160, 120, gs.background.pos.x, gs.background.pos.y
				);
				draw_sprite_from_atlas(286, 70, 95, 26, 5, 7); 
				
				draw_sprite_from_atlas(184, 199, 89, 24, 6, 97);
				
				//drawNumbers(gss2);
				//drawNumbers(gss1);
				break;
			case background_GAME:
				draw_sprite_from_atlas(
					0, 100, 160, 120, gs.background.pos.x, gs.background.pos.y
				);
				drawNumbers(gss2);
				drawNumbers(gss1);
				break;
			case background_GAME_OVER:
				draw_sprite_from_atlas(
				0, 100, 160, 120, gs.background.pos.x, gs.background.pos.y
				);
				if (score <= 10){ 
					draw_sprite_from_atlas(292, 194, 113, 57, 45, 20);
					draw_sprite_from_atlas(246, 77, 22, 22, 58, 41);
					
					game_score_state_t gss1pom;
					gss1pom.score.pozicija.x = 140;
					gss1pom.score.pozicija.y = 38;
					gss1pom.score.animScore.scoreState = gss1.score.animScore.scoreState;
					gss1pom.score.animScore.brojac = gss1.score.animScore.brojac;
					
					game_score_state_t gss2pom;
					
					gss2pom.score.pozicija.x = 130;
					gss2pom.score.pozicija.y = 38;
					gss2pom.score.animScore.scoreState = gss2.score.animScore.scoreState;
					gss2pom.score.animScore.brojac = gss2.score.animScore.brojac;
					
					drawNumbersSmall(gss1pom);
					drawNumbersSmall(gss2pom);
				} else {
					draw_sprite_from_atlas(292, 194, 113, 57, 45, 20);
					draw_sprite_from_atlas(246, 101, 22, 22, 58, 41);
					
					game_score_state_t gss1pom;
					gss1pom.score.pozicija.x = 140;
					gss1pom.score.pozicija.y = 38;
					gss1pom.score.animScore.scoreState = gss1.score.animScore.scoreState;
					gss1pom.score.animScore.brojac = gss1.score.animScore.brojac;
					
					game_score_state_t gss2pom;
					
					gss2pom.score.pozicija.x = 130;
					gss2pom.score.pozicija.y = 38;
					gss2pom.score.animScore.scoreState = gss2.score.animScore.scoreState;
					gss2pom.score.animScore.brojac = gss2.score.animScore.brojac;
					
					drawNumbersSmall(gss1pom);
					drawNumbersSmall(gss2pom);
				}
				
				int jedBest = 0;
				int desBest = 0;
				jedBest = best_score % 10;
				desBest = best_score / 10;
				if (desBest == 0)
					desBest = 10;
				
				game_score_state_t gssBestJ;
				gssBestJ.score.pozicija.x = 140;
				gssBestJ.score.pozicija.y = 58;
				gssBestJ.score.animScore.scoreState = jedBest;
				
				drawNumbersSmall(gssBestJ);
				
				game_score_state_t gssBestD;
				gssBestD.score.pozicija.x = 130;
				gssBestD.score.pozicija.y = 58;
				gssBestD.score.animScore.scoreState = desBest;
				
				drawNumbersSmall(gssBestD);
				
				draw_sprite_from_atlas(420, 42, 19, 16, 20, 40); //mrtva ptica
				
				draw_sprite_from_atlas(184, 172, 96, 21, 0, 98); //game over
				draw_sprite_from_atlas(2, 233, 54, 20, 100, 100); //looser
				break;
		}
		
		if(gs.background.anim.state != background_GAME_OVER) {
			//iscrtaj ticu
			switch(gbs.bird.animBird.birdState) {
				case BIRD_IDLE:
				case BIRD_OPENING_WING_UP:
				case BIRD_WING_MIDDLE: //down
				
					draw_sprite_from_atlas(
						413, 212, 17, 12 , gbs.bird.pozicija.x, gbs.bird.pozicija.y
					);
					
					break;
					
				case BIRD_WING_UP: //middle
				
					draw_sprite_from_atlas(
						413, 238, 17, 12, gbs.bird.pozicija.x, gbs.bird.pozicija.y
					);
					
					break;
					
				case BIRD_WING_DOWN: //up
			
					draw_sprite_from_atlas(
						413, 186, 17, 12, gbs.bird.pozicija.x, gbs.bird.pozicija.y
					);
					break;
			}	
		}

			//iscrtaj cevcugu//
		if(gs.background.anim.state != background_GAME_OVER) {
			 switch(gts.tube.tube_anim.tubeState){
				case TUBE_BEGIN:
					
					draw_sprite_from_atlas(
						184, 145-35, 26, 17+35, gts.tube.pozicija.x, 0
					);

					draw_sprite_from_atlas(
						212, 2, 26, 17+0, gts.tube.pozicija.x, gts.tube.pozicija.y
					);	
					break;
					
				case TUBE_GO:	
					// prva dva je gde pocinju na slici
					// druga dva su dimenzija same cevi
					// treca dva su pozicije koje se iscrtavaju
					
					draw_sprite_from_atlas(
						184, 145-rnd_par, 26, 17+rnd_par, gts.tube.pozicija.x, 0
					);

					draw_sprite_from_atlas(
						212, 2, 26, 17+rnd, gts.tube.pozicija.x, gts.tube.pozicija.y-rnd
					);
			}
		}
		if (score >= 5 ) {
		
			if (gs.background.anim.state != background_GAME_OVER) {
				//iscrtaj drugu cevcugu//
					switch(gts2.tube.tube_anim.tubeState){
						case TUBE_BEGIN:

							draw_sprite_from_atlas(
								184, 145-35, 26, 17+35, gts2.tube.pozicija.x, 0
							);

							draw_sprite_from_atlas(
								212, 2, 26, 17+0, gts2.tube.pozicija.x, gts2.tube.pozicija.y
							);	
							break;
							
						case TUBE_GO:	
							
							draw_sprite_from_atlas(
								184, 145-rnd_par2, 26, 17+rnd_par2, gts2.tube.pozicija.x, 0
							);

							draw_sprite_from_atlas(
								212, 2, 26, 17+rnd2, gts2.tube.pozicija.x, gts2.tube.pozicija.y-rnd2
							);
					}
			}
		}
		

		//iscrtaj skor
		
		
		//loser
		//draw_sprite_from_atlas(2, 233, 54, 20, 60, 100);
		
	}

	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
