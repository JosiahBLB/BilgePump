/*
 * Bilge_Pump.cpp
 *
 * Created: 2/03/2022 8:14:26 pm
 * Author : qhs7289
 */ 

#define pumpOn (PORTC |= (1<<0))
#define pumpOff	(PORTC &= ~(1<<0))
#define senHI (PINA & 0x01)
#define senLO (PINA & 0x02)


int main(void)
{
    while (1) 
    {
		  if(senHI)
			  pumpOn;
		  if(senLO)
			  pumpOff;
    }
}
