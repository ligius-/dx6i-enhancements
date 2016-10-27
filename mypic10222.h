static volatile       bit	TMR07		@ ((unsigned)&TMR0*8)+7;
//---------------------------------------------------------
void send_serial_byte(byte data)
{
  // this manually sends a serial byte out any PIC pin.
  // NOTE! serial is inverted to connect direct to PC serial port.
  // baud timing is done by using TMR1L and removing
  // timer error after each baud.
  byte i;

  i=8;                            // 8 data bits to send

  PIN_SER_OUT = 0;                // make start bit
  TMR0 = (256 - SER_BAUD);       // load TMR1 value for first baud;
  while (TMR07);                // wait for baud

  while(i)                        // send 8 serial bits, LSB first
  {
    //if(data.0) PIN_SER_OUT = 0;  // invert and send data bit
    if(BITVAL(data,0)) PIN_SER_OUT = 1;  // invert and send data bit
    else    PIN_SER_OUT = 0;

    data = (data >> 1);           // rotate right to get next bit
    i--;
    TMR0 -= SER_BAUD;            // load corrected baud value
    while(TMR07);              // wait for baud
  }

  PIN_SER_OUT = 1;                // make stop bit
  TMR0 -= SER_BAUD;              // wait a couple of baud for safety
  while(TMR07);
  TMR0 -= SER_BAUD;
  while(TMR07);
}


/* Sample code to set up the A2D module */
/*void init_a2d(){
	#if ADCIN==GP0
		ANS0 = 1;
		ANS1 = 0;
	#else
		ANS0 = 0;
		ANS1 = 1;
	#endif

	#if ADCIN==GP0
		CHS0 = 0;
	#else
		CHS0 = 1;
	#endif
	
	CHS1 = 0;
	ADON=1;
}
*/
#define init_a2d(){\
	ANS0 = 0;\
	ANS1 = 1;\
	CHS0 = 1;\
	CHS1 = 0;\
	ADON=1;\
}\

#define read_a2d(){	GO_nDONE = 1;	while(GO_nDONE)continue;}

#define delay_s(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4.0)))

