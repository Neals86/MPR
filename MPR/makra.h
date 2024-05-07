
/***************************************************************************
*  
*   Soubor:       makra.h
*
*   Obsah:        definice maker pro bitove operace
*                 
*   Vytvoreno:    26.5.2013
*
***************************************************************************/
//makra pro bitove operace 

#define  SETBIT(BIT_POLE,BIT) (BIT_POLE|=(1<<BIT))       //nahozeni bitu
#define  CLRBIT(BIT_POLE,BIT) (BIT_POLE &= ~(1<<BIT))    //nulovani bitu
#define  TESTBIT(BIT_POLE,BIT) (BIT_POLE & (1<<BIT))     //test nahozeni bitu
#define  TESTNEGBIT(BIT_POLE,BIT) (~BIT_POLE & (1<<BIT)) //test nuloveho bitu
#define  NEGBIT(BIT_POLE, BIT)  (BIT_POLE ^= (1<<(BIT))) //negace bitu 

