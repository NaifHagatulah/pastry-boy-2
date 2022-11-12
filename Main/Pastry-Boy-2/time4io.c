#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(void)
{
    return (PORTD & 0xF00) >> 8; // returnerar i de 4 least significant bitsen 1or och nollor beroende på om switch är nere eller inte PIC_32 fixar redan så de är en derefrencerad perkare dvs värdet finns dirket för hitta adress skriv & (sökt värde finns i de 4 bits i index 7 och fram fyra bits så shifta med 8 för få rätt värden)
}
int getbtns(void) // samma fast med knapparna nu finns de endast tre kappar inte 4 switches
{
    return (PORTD & 0xE0) >> 4; // shifta 4 steg då de är på index 4 och 4 bits fram dvs index 7
}