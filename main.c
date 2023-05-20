#include <msp430.h>

#define S0_PIN BIT4
#define S1_PIN BIT3
#define S2_PIN BIT1
#define S3_PIN BIT2
#define OUT_PIN BIT2

int r=0,g=0,b=0;

void RGB_ON(unsigned char tR,unsigned char tG, unsigned char tB) // rgb ledin yanmasý ve pwm ayarý yapýlmasý
{
    unsigned char a;
    unsigned int i;

    for(a=0;a<25;a++)
    {
            for(i=1;i<=255;i++)
            {
                if(i<=tR)P1OUT|=BIT5;
                if(i>tR)P1OUT&= ~BIT5;

                if(i<=tG)P1OUT|=BIT4;
                if(i>tG)P1OUT&= ~BIT4;

                if(i<=tB)P1OUT|=BIT6;
                if(i>tB)P1OUT&= ~BIT6;

                __delay_cycles(1);
            }
    }
}

void Colour_Picker(int r ,int g ,int b) //sensorden aldýðýmýz verilere göre renk aralýklarýný belirliyoruz ona göre rbgled yakýyoruz
{
 	if((30<r && r<44) && (17<g && g<25) && (57<b && b<70))
  		RGB_ON(255,0,0);//kirmizi
 	else if ((15<r && r<25) && (10<g && g<17) && (55<b && b<65))
  		RGB_ON(0,255,0);//yesil
 	else if ((68<r && r<75) && (15<g && g<25) && (40<b && b<50))
    	RGB_ON(0,0,255);//mavi
 	else if ((20<r && r<33) && (12<g && g<16) && (50<b && b<57))
    	RGB_ON(255,255,0);//sari
 	else if ((47<r && r<55) && (16<g && g<25) && (42<b && b<55))
    	RGB_ON(169,0,200);//mor
	else if ((10<r && r<27) && (10<g && g<21) && (20<b && b<30))
    	RGB_ON(255,255,255);//beyaz
	else if ((30<r && r<38) && (10<g && g<21) && (20<b && b<30))
    	RGB_ON(255,0,255);//pembe
 	else if ((50<r && r<65) && (20<g && g<31) && (64<b && b<80))
 	{
    	RGB_ON(0,0,255); // kahverengi
    	__delay_cycles(10000);
    	RGB_ON(0,0,0);
    	__delay_cycles(10000);
 	}
	else if ((54<r && r<65) && (15<g && g<21) && (45<b && b<53))
	{//gri
    	RGB_ON(0,255,0);
     	_delay_cycles(10000);
     	RGB_ON(0,0,0);
     	__delay_cycles(10000);
 	}	
}

unsigned long pulseIn(unsigned int pin, unsigned int state) // pin üzerindeki LOW süresini okur
{
    unsigned long pulseWidth = 0;

    while ((P1IN & pin) != state)
    {
		
    }
    while ((P1IN & pin) == state)
	{
    	pulseWidth++; // Pulse süresini ölçmek için sayaç
	}

    return pulseWidth;
}


void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Watchdog Timer'ý devre dýþý býrak

    P2DIR = 1;
    P1DIR = 0x00;
    P1OUT = 0x00;
    P2OUT = 0x00;

    P2DIR |= S0_PIN + S1_PIN + S2_PIN + S3_PIN; 

    P1DIR |= BIT4 + BIT5 + BIT6;
    P1DIR &= ~OUT_PIN;

    P2OUT |= S0_PIN;

    while(1)
    {
        P2OUT &= ~S2_PIN;	//kýrmýzý filtrelemek için
        P2OUT &= ~S3_PIN;

        r = pulseIn(OUT_PIN, OUT_PIN);
        __delay_cycles(100);

        P2OUT |= S2_PIN;	// yesili filtrelemek için
        P2OUT &= ~S3_PIN;

        g = pulseIn(OUT_PIN, OUT_PIN);

        __delay_cycles(100);

        P2OUT &= ~S2_PIN;	// maviyi filtrelemek için
        P2OUT |= S3_PIN;

        b = pulseIn(OUT_PIN, OUT_PIN);

        Colour_Picker(r, g, b);

        __delay_cycles(10000);
    }
}


