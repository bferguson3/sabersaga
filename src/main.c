
#include <stdio.h>
#include <gb/font.h>
#include <string.h>
#include <gb/gb.h>
#include <gb/cgb.h>

#include "..\res\ninja01.h"

volatile unsigned char* BG_PAL_REG = (unsigned char*)0xff47; // gbbw select register 
volatile unsigned char* SPR0_PAL_REG = (unsigned char*)0xff48;
volatile unsigned char* SPR1_PAL_REG = (unsigned char*)0xff49; // ONE BYTE each 11 10 01 00 

volatile unsigned char* BG_PAL_SELECT_REG = (unsigned char*)0xff68; // gbc select register 
volatile unsigned char* BG_PAL_DATA_REG = (unsigned char*)0xff69; // gbc pal out register
volatile unsigned char* SPR_PAL_SELECT_REG = (unsigned char*)0xff6a; // gbc select register 
volatile unsigned char* SPR_PAL_DATA_REG = (unsigned char*)0xff6b; // gbc pal out register
//color
#define SetBGColor(palette, colno, color) *BG_PAL_SELECT_REG = (UBYTE)(0x80 | (palette << 3) | (colno << 1) | 0);\
                                    *BG_PAL_DATA_REG = (UBYTE)(color & 0xff);\
                                    *BG_PAL_DATA_REG = (UBYTE)((color & 0xff00) >> 8);
#define RGB15(r,g,b) (int)(r | g << 5 | b << 10)

#define QUEUE_SIZE 25
volatile void* function_q[QUEUE_SIZE];
volatile int q_args[QUEUE_SIZE];
int q_in = 0;
int q_ofs = 0;
int GLOBALWAIT = 0;
void AddQueue(void *q, int a);
#define AddQ(s) AddQueue(s, 0)
void DoQ();
#define secs(s) (int)(s*60)

void Wait(int a)
{
    GLOBALWAIT += a;
}

void AddQueue(void *q, int a)
{
    function_q[q_ofs] = q;
    q_args[q_ofs] = a;
    q_ofs += sizeof(void*);
    if(q_ofs > QUEUE_SIZE * sizeof(void*)) q_ofs = 0;
}

void DoQ()
{
    void(*f)(int) = function_q[q_in];
    if(f != NULL){ 
        f(q_args[q_in]);
        q_in += sizeof(void*);
        if(q_in > QUEUE_SIZE * sizeof(void*)) q_in = 0;
    }
}

// Normal game shit
enum GameScreen { TITLE_SCREEN } GAME_SCREEN;
void LoadTitleScreen()
{
    GAME_SCREEN = TITLE_SCREEN;
    printf("\n\n\t\tSABER SAGA\n\n\t\tPress Start\n");
}
void Test2()
{
    printf("wtf i love anal");
}

void main(void)
{
    // Clear Queue
    memset(&function_q, NULL, sizeof(void*) * QUEUE_SIZE);

    // IF GAMEBOY: init font
    font_init();
    font_t ibm_font;
    ibm_font = font_load(font_ibm);  /* 96 tiles */
    font_set(ibm_font);
    
    // Setup title test
    //mode(get_mode() | M_NO_SCROLL);
    AddQueue(&Wait, 25);
    AddQ(&LoadTitleScreen);
    AddQueue(&Wait, secs(2.5));
    AddQ(&Test2);
    
    // Set palette, or black if on GBBW
    disable_interrupts();
    DISPLAY_OFF
    SetBGColor(0, 0, RGB15(19, 15, 31));
    SetBGColor(0, 3, RGB15(10, 29, 8));
    //bw
    *BG_PAL_REG = 0xff;
    DISPLAY_ON;
    enable_interrupts();
    

    while(1)
    {
        // Update:
        UPDATE:
        if(GLOBALWAIT > 0)
        {   // Am i in Wait() ?
            GLOBALWAIT--;
            goto DRAW;
        }
  
        DoQ();
        
        // Draw:
        DRAW:
        wait_vbl_done();
    }
}