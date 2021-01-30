
#include <stdio.h>
#include <gb/font.h>
#include <string.h>
#include <gb/gb.h>
#include <gb/cgb.h>

#define RGB15(r,g,b) (int)(r | g << 5 | b << 10)

#define true 1
#define false 0
#define null NULL

#include "sabergfx.h"

// BWGB registers
volatile unsigned char* BG_PAL_REG = (unsigned char*)0xff47; // gbbw select register 
volatile unsigned char* SPR0_PAL_REG = (unsigned char*)0xff48;
volatile unsigned char* SPR1_PAL_REG = (unsigned char*)0xff49; // ONE BYTE each 11 10 01 00 
// CGB registers
volatile unsigned char* BG_PAL_SELECT_REG = (unsigned char*)0xff68; // gbc select register 
volatile unsigned char* BG_PAL_DATA_REG = (unsigned char*)0xff69; // gbc pal out register
volatile unsigned char* SPR_PAL_SELECT_REG = (unsigned char*)0xff6a; // gbc select register 
volatile unsigned char* SPR_PAL_DATA_REG = (unsigned char*)0xff6b; // gbc pal out register

volatile UBYTE* cgbvram_reg = (UBYTE*)0xff4f;
volatile UBYTE* cgbvram = (UBYTE*)0x9800;
volatile UBYTE* LCD_CTRL_REG = (UBYTE*)0xff40;
#define CGB_VRAM_ON *cgbvram_reg = 1;
#define CGB_VRAM_OFF *cgbvram_reg = 0;
#define TOGGLESCREEN *LCD_CTRL_REG ^= 0x80;
#define CGB_ATTRIBUTE(Priority, Yflip, Xflip, BWPal, VRAMbank, ColorPal) \
             (UBYTE)((Priority << 7) | (Yflip << 6) | (Xflip << 5) | (BWPal << 4)\
              | (VRAMbank << 3) | (ColorPal))

// Color
#define SetBGColor(palette, colno, color) *BG_PAL_SELECT_REG = (UBYTE)(0x80 | (palette << 3) | (colno << 1) | 0);\
                                    *BG_PAL_DATA_REG = (UBYTE)(color & 0xff);\
                                    *BG_PAL_DATA_REG = (UBYTE)((color & 0xff00) >> 8);
#define SetSPRColor(palette, colno, color) *SPR_PAL_SELECT_REG = (UBYTE)(0x80 | (palette << 3) | (colno << 1) | 0);\
                                    *SPR_PAL_DATA_REG = (UBYTE)(color & 0xff);\
                                    *SPR_PAL_DATA_REG = (UBYTE)((color & 0xff00) >> 8);

// Sprites - fix later    
#define SetSprAttr16(a, b) set_sprite_prop(a, b);\
                    set_sprite_prop(a+1, b);\
                    set_sprite_prop(a+2, b);\
                    set_sprite_prop(a+3, b);
#define SetSprTile16(a, b) set_sprite_tile(a, b);\
                    set_sprite_tile(a+1, b+1);\
                    set_sprite_tile(a+2, b+2);\
                    set_sprite_tile(a+3, b+3);
#define MoveSpr16(a, x, y) move_sprite(a, x, y);\
                    move_sprite(a+1, x+8, y);\
                    move_sprite(a+2, x, y+8);\
                    move_sprite(a+3, x+8, y+8);
#define OAM_FLUSH __asm \
                    ld a,#0xcf ; source of sprite oam - cf00 to cfff \
                    ld  (0xFF46),a \
                    ld  a,#0x28 \
                00009$: \
                    dec a \
                    jr  nz, 00009$ \
                __endasm;
//

///////////////////////////////
// Queue 
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
//////////////////////////////


// Frame data
#define offset_a 16
const unsigned char ninja01_f[] = { 0+offset_a, 1+offset_a, 2+offset_a, 3+offset_a };
const unsigned char ninja02_f[] = { 4+offset_a, 5+offset_a, 6+offset_a, 7+offset_a };


///////////////////////////////
// Normal game shit
void TitleInputHandler();
void BlankBG();
void(*ActiveJoyHandler)();
enum GameScreen { TITLE_SCREEN, WORLDMAP } GAME_SCREEN;
void TitleInputHandler()
{
    UINT8 j = joypad();
    if(j & J_START)
    {
        GAME_SCREEN = WORLDMAP;
        AddQ(&BlankBG);
    }
}
void LoadTitleScreen()
{
    GAME_SCREEN = TITLE_SCREEN;
    printf("\n\n\t\tSABER SAGA\n\n\t\tPress Start\n");
}
void Test2()
{
    printf("wtf i lan");
}
void Test3()
{
    //SetSprTile16(0, 4);
    ActiveJoyHandler = (void*)&TitleInputHandler;
}
///////////////////////////////


void BlankBG()
{
    wait_vbl_done(); // wait til draw
    volatile int* bg = (unsigned int*)0x9800;
    TOGGLESCREEN
    for(int i = 0; i < 0x1ff; i++){
        *bg = 0;
        bg++;
    }
    wait_vbl_done();
    TOGGLESCREEN
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
    
    // Set palette, or black if on GBBW
    disable_interrupts();
    TOGGLESCREEN
    SHOW_SPRITES;

    for(UBYTE i = 0; i < 4; i++){
        SetBGColor(0, i, ninjapal[i]);
        SetSPRColor(0, i, priestpal[i]);
    }
    //bw
    *BG_PAL_REG = 0xff;
    set_bkg_data(16, 8, (void*)&ninja01);
    set_bkg_tiles(0, 0, 2, 2, (void*)&ninja01_f);
    
    set_sprite_data(0, 8, (void*)&priest01);


    CGB_VRAM_ON
    //*cgbvram = CGB_ATTRIBUTE(false, false, false, 0, 0, 1);// change palette of tile 0 to 01
    CGB_VRAM_OFF

    TOGGLESCREEN
    enable_interrupts();
    
    // Setup title test
    //mode(get_mode() | M_NO_SCROLL);
    AddQueue(&Wait, 25);
    AddQ(&LoadTitleScreen);
    AddQueue(&Wait, secs(2.5));
    AddQ(&Test2);
    AddQueue(&Wait, secs(1.2));
    AddQ(&Test3);
    
    while(1)
    {
        /////// Update:
        UPDATE:
        if(GLOBALWAIT > 0)
        {   // Am i in Wait() ?
            GLOBALWAIT--;
            goto DRAW;
        }
        //// Input
        if(ActiveJoyHandler != null) ActiveJoyHandler();

        // Engage queue
        DoQ();
        
        /////// Draw:
        DRAW:
        wait_vbl_done();
        disable_interrupts();
        //DISPLAY_OFF;
        // Swap graphic data for sprites!

        OAM_FLUSH
        //DISPLAY_ON;
        enable_interrupts();
    }
}