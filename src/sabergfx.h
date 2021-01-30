#ifndef sabergfx

#include "..\res\ninja01.h"
#include "..\res\ninja02.h"
#include "..\res\priest01.h"
#include "..\res\priest02.h"
#include "..\res\soldier01.h"
#include "..\res\soldier02.h"
#include "..\res\alien01.h"
#include "..\res\alien02.h"

#define NINJAPAL_C0 RGB15(31, 31, 31)
#define NINJAPAL_C1 RGB15(1, 1, 20)
#define NINJAPAL_C2 RGB15(1, 1, 8)
#define NINJAPAL_C3 RGB15(16, 29, 30)
const int ninjapal[] = {
    RGB15(31, 31, 31), RGB15(1, 1, 20), NINJAPAL_C2, NINJAPAL_C3
};
#define PRIESTPAL_C0 RGB15(31, 31, 31)
#define PRIESTPAL_C2 RGB15(3, 1, 8)
#define PRIESTPAL_C1 RGB15(3, 16, 22) //swap 2 and 1
#define PRIESTPAL_C3 RGB15(17, 31, 16)
const int priestpal[] = {
    PRIESTPAL_C0, PRIESTPAL_C1, PRIESTPAL_C2, PRIESTPAL_C3
};
#define SOLDRPAL_C0 RGB15(31, 31, 31)
#define SOLDRPAL_C2 RGB15(1, 5, 1)
#define SOLDRPAL_C1 RGB15(1, 16, 1) 
#define SOLDRPAL_C3 RGB15(16, 1, 1)

#define ALIENPAL_C0 RGB15(31, 31, 31)
#define ALIENPAL_C2 RGB15(1, 3, 8)
#define ALIENPAL_C1 RGB15(6, 11, 13) 
#define ALIENPAL_C3 RGB15(1, 26, 14)

#endif