
#include <FreqPeriod.h>
//#include <wiring.h>
//#include <pins_arduino.h>



volatile unsigned char  FreqPeriod::f_capt;
volatile unsigned int FreqPeriod::capta;
volatile unsigned long int FreqPeriod::captd;
volatile int FreqPeriod::ocnt;

void FreqPeriod::begin(){

	
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__)
	 
  pinMode(5,OUTPUT);



  TCCR1A =0;
  TCCR1B =0;

  ADCSRB &= ~(1<<ACME);
  ACSR &= ~(1<<ACD) ;    // Analog Comparator disable off
  // ACSR |= (1<<ACBG);   // Analog Comparator Bandgap reference on
  ACSR |= (1<<ACIE);   // Analog Comparator Interrupt enable
  ACSR |= (1<<ACIC);

  ACSR &= ~(1<<ACIS0); // comparator detection edge
  ACSR |= (1<<ACIS1);  // comparator detection edge

  //  CS12 CS11 CS10 Description
  //    0   0    0   No clock source (Timer/Counter stopped).
  //    0   0    1   clkI/O/1 (No prescaling)
  //    0   1    0   clkI/O/8 (From prescaler)
  //    0   1    1   clkI/O/64 (From prescaler)
  //    1   0    0   clkI/O/256 (From prescaler)

  TCCR1B |= (1<<CS10); // set prescaler to 16 MHz
  TCCR1B &= ~(0<<CS11);
  TCCR1B &= ~(0<<CS12);

  TCCR1B |= (1<<ICNC1);  // input noise canceler on
  TCCR1B &= ~(1<<ICES1); // input capture edge select =0 



  DIDR1 |= (1<< AIN0D);  // disable digital input buffer AIN0/1
  DIDR1 |= (1<< AIN1D);

  TIMSK1 |= (1<<TOIE1);  // Timer/Counter1, Overflow Interrupt Enable

 // TIMSK0 &= ~(1<<TOIE0); // Timer/Counter0, Overflow Interrupt disable
	
#endif
	

}
//***************************************************************************
unsigned long int  FreqPeriod::getPeriod() {
      unsigned long int rr= 0; 
  if ( FreqPeriod::f_capt){
    FreqPeriod::f_capt=0;
    rr= FreqPeriod::captd;
}
return(rr);
}


//***************************************************************************
// Timer1 Overflow Interrupt Service
ISR(TIMER1_OVF_vect   ) {
  FreqPeriod::ocnt++;  // count number of timer1 overflows
}
//***************************************************************************
// Analog Comparator Interrupt Service
ISR(ANALOG_COMP_vect  ) {

  if (!(ACSR &  (1<<ACIS0))) {  // comparator falling edge
    digitalWrite(5,0);          // reduce comparator threshold level on AIN0
    ACSR |= (1<<ACIS0);         // next comparator detection on rising edge 

					  // compute period  length  timer1 capture value ,  account timer1 numer of overflows 	
    FreqPeriod::captd= ICR1 + FreqPeriod::ocnt* 0x10000; 

					  // compute period difference 
    FreqPeriod::captd=FreqPeriod::captd-FreqPeriod::capta; 
          
    FreqPeriod::capta=ICR1;     // store capture value
    FreqPeriod::ocnt=0;         // reset number of timer1 overflows
    FreqPeriod::f_capt=1;       // measure ready flag
  }   
  else  {
    ACSR &= ~(1<<ACIS0);         //  next comparator detection on falling edge 
    digitalWrite(5,1);           // elevate comparator threshold level on AIN0
  }
}


