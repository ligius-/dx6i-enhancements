#include <pic.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

#if _XTAL_FREQ==4000000
#define OPTMEGA IOSCFS_4MHZ
#elif _XTAL_FREQ==8000000
#define OPTMEGA IOSCFS_8MHZ
#else
#error Invalid XTAL FREQ
#endif
//TODO: POR, BOR, WDT(?)
//__CONFIG(PROTECT & WDTDIS & OPTMEGA & MCPUEN & MCLRDIS);
__CONFIG(CP_ON & WDT_OFF & OPTMEGA & MCPU_ON & MCLRE_OFF);
//t0cs = 0 => int clock; psa=0 => prescaler>timer0

#define SER_BAUD 52				// TMR1 (1Mhz/19200 baud) = 52

//DEFINE WHAT PORTS SHOULD BE USED
#define PIN_SER_OUT GP2			// which pin for serial out (PORTA.F3)
#define TRAINER GP0
#define ADCIN GP1				
#define BACKLIGHT GP2
#define ROLLER GP3

#define SW_ON	0
#define SW_OFF	1

#define OPERATIONAL_DELAY 95                           //SW will check the changes at each x ms
#define VIDLE 40					//0.5V/3.3V*255
#define VMDL 115					//1.5V/3.3V*255
#define VDISABLED 220					//something high
#define HISTER 30					//>0.2V/3.3V*255
#define VMDL_LOW VMDL-HISTER
#define VMDL_HIGH VMDL+HISTER
#define IDLE_TIME_WARNING (900)               //idle timer warning after x/10 seconds of inactivity
#define BACKLIGHT_TIME (50)               //backlight off after x/10 s of inactivity

//MODE DEFINITIONS
#define FIXED 0
#define CP 1
#define DISABLED 2

//helper definitions for timer
//1:256 @ 4Mhz / 4 = 1000000/256 = 3096.25Hz TMR0 tick = 256us; 3096/256=12.0947265625Hz=65.536ms TMR0 rollover
#define TMR_PRESCALER 256
#define TICK_FREQ _XTAL_FREQ/4/TMR_PRESCALER
#define TMR0_ROLLOVER_FREQ TICK_FREQ/256
#define MINUTE 60

#include "commonpic.h"
#include "mypic10222.h"

byte mode = 0; //0=fixed; 1=CP
bit active = 0;
bit toggle_active = 0;
bit last_roller = 0;
int idle_time;

void pulse_trainer() {
    TRAINER = SW_ON;
    __delay_ms(95);
    TRAINER = SW_OFF;
    __delay_ms(95);
}

void main(void) {
    //OPTION = (GPWU | GPPU | PS0 | PS1 | PS2);//1:256 TMR prescale rate
    OPTION = (nGPWU | nGPPU); //1:1 TMR prescale rate //TODO: check these

    init_a2d(); // initialise the A2D module
    TRIS = 0b1010; //gp0 out, gp1 in, gp2 out, gp3 in
    BACKLIGHT = 1;

    delay_s(1);
    read_a2d();

    if (ADRES < VIDLE) {
        mode = FIXED;
    } else if (ADRES > VDISABLED) {
        mode = DISABLED;
    } else {
        mode = CP;
    }

    active = 0;
    last_roller = ROLLER;

    for (; mode == DISABLED;) {
        //DEAD
    }

    for (;;) {
        if (mode == FIXED) {
            if (ADRES < VIDLE) {
                if (active) {
                    toggle_active = 1;
                }
            } else {
                if (!active) {
                    toggle_active = 1;
                }
            }
        } else { //COLLECTIVE PITCH MODE
            if (ADRES > (VMDL_LOW) && ADRES < (VMDL_HIGH)) {//TODO: overflow check
                if (active) {
                    toggle_active = 1;
                }
            } else {
                if (!active) {
                    toggle_active = 1;
                }
            }
        }

        if (toggle_active) {
            active = !active;
            pulse_trainer();
            toggle_active = 0;
            idle_time = 0;
        } else if (last_roller != ROLLER) {
            last_roller = ROLLER;
            idle_time = 0;
            BACKLIGHT = 1;
        } else if (!active){
            //crude way of ensuring a cycle takes 100ms since that is a lot longer than all other operations
            __delay_ms(OPERATIONAL_DELAY);
            idle_time++;
        }

        if (idle_time >= BACKLIGHT_TIME) {
            BACKLIGHT = 0;
        }
        if (idle_time >= IDLE_TIME_WARNING) {
            idle_time = 0;
            pulse_trainer();pulse_trainer();
        }

        read_a2d(); //takes 13Tad so time is 13/(4Mhz/4) = 13us
    }
}
