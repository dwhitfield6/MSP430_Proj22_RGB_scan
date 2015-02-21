#include <msp430.h>

#define audio1Input INCH_0
#define audio2Input INCH_4
#define audio3Input INCH_5

#define GREEN BIT1
#define RED BIT3
#define BLUE BIT2

unsigned int analogRead(unsigned int pin);

unsigned int GREENPWM =0;
unsigned int GREENPWMtemp =0;
unsigned int REDPWM =0;
unsigned int REDPWMtemp =1;
unsigned int BLUEPWM =0;
unsigned int BLUEPWMtemp =1;
int audio1level =0;
int audio2level =0;
int audio3level =0;
int audio1levelfix =0;
int audio2levelfix =0;
int audio3levelfix =0;
unsigned int count =0;
unsigned int BLUEArray =0;
unsigned int GREENArray =0;
unsigned int REDArray =0;


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    P1DIR |= (GREEN + RED + BLUE);
    P1OUT |= (GREEN + RED + BLUE);

    TA0CCTL0 |= CCIE;                             // CCR0 interrupt enabled
    TA0CTL = TASSEL_2 + MC_1 + ID_0;           // SMCLK/8, upmode
    TA0CCR0 =  90;

    __enable_interrupt();

       while(1)
       {


    	   BLUEPWMtemp++;
    	   if(BLUEPWMtemp > 20)
    	   {
    		   GREENPWMtemp++;
    		   BLUEPWMtemp =0;
    	   }
    	   if(GREENPWMtemp>20)
    	   {
    		   GREENPWMtemp=0;
    		   REDPWMtemp++;
    	   }
    	   if(REDPWMtemp > 20)
    	   {
    		   REDPWMtemp =0;
    	   }

                          _delay_cycles(2000000);

       }

}

unsigned int analogRead(unsigned int pin) {
 ADC10CTL0 = ADC10ON + ADC10SHT_1 + SREF_1 + REFON;
 ADC10CTL1 = ADC10SSEL_0 + pin;
 if (pin==INCH_0){
 ADC10AE0 = 0x01;
 }
 else if (pin==INCH_1){
 ADC10AE0 = 0x02;
 }
 else if (pin==INCH_2){
 ADC10AE0 = 0x04;
 }
 else if (pin==INCH_3){
  ADC10AE0 = 0x08;
  }
 else if (pin==INCH_4){
  ADC10AE0 = 0x10;
  }
 else if(pin==INCH_5){
 ADC10AE0 = 0x20;
 }
 else if(pin==INCH_6){
  ADC10AE0 = 0x40;
  }
 else if(pin==INCH_7){
  ADC10AE0 = 0x80;
  }
 ADC10CTL0 |= ENC + ADC10SC;
 //_delay_cycles(10);
 while (1) {
 if (((ADC10CTL0 & ADC10IFG)==ADC10IFG)) {
 ADC10CTL0 &= ~(ADC10IFG +ENC);
 break;
 }
 }
 return ADC10MEM;
 }

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
    count++;
    if(count > 46)
    {

        BLUEPWM = BLUEPWMtemp;
        REDPWM = REDPWMtemp;
        GREENPWM = GREENPWMtemp;

 	   if(REDPWM != 0)
 	   {
 		   P1OUT &= ~(RED); //red on
 	   }
 	   if(GREENPWM != 0)
 	   {
 		   P1OUT &= ~(GREEN); //green on
 	   }
 	   if(BLUEPWM != 0)
 	   {
 		   P1OUT &= ~(BLUE); //blue on
 	   }
 	   count =0;
    }
    if(GREENPWM == count)
    {
 	   P1OUT |= GREEN;
    }
    if(BLUEPWM == count)
    {
 	   P1OUT |= BLUE;
    }
    if(REDPWM == count)
    {
 	   P1OUT |= RED;
    }

}
