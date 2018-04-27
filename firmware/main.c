#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/pgmspace.h>
#include "usbdrv/usbdrv.h"
#include "usbdrv/oddebug.h"
#include "../commands.h" /* common ioctl commands (requests) */

#define DIR_PORTB DDRB /* port attached to page-motor */
#define DIR_PORTC DDRC /* port attached to char-motor & servo */
#define STEP1_PORT PORTB
#define STEP2_PORT PORTC
#define SERVO_PORT PORTC
#define STEP_BIT1 (1<<1) /*stepper A*/
#define STEP_BIT2 (1<<2) /*stepper B*/
#define STEP_BIT3 (1<<3) /*stepper C*/
#define STEP_BIT4 (1<<4) /*stepper D*/
#define SERVO_BIT (1<<5) /*servo control*/

void pageForward() /* move page-motor forward by 1 step in 4ms */
{
STEP1_PORT |= (STEP_BIT1|STEP_BIT3);
STEP1_PORT &= ~(STEP_BIT2|STEP_BIT4);
_delay_ms(1);
STEP1_PORT |= (STEP_BIT2|STEP_BIT3);
STEP1_PORT &= ~(STEP_BIT1|STEP_BIT4);
_delay_ms(1);
STEP1_PORT |= (STEP_BIT2|STEP_BIT4);
STEP1_PORT &= ~(STEP_BIT1|STEP_BIT3);
_delay_ms(1);
STEP1_PORT |= (STEP_BIT1|STEP_BIT4);
STEP1_PORT &= ~(STEP_BIT2|STEP_BIT3);
_delay_ms(1);
}
void pageBackward() /* move page-motor backward by 1 step in 4ms */
{
STEP1_PORT |= (STEP_BIT1|STEP_BIT4);
STEP1_PORT &= ~(STEP_BIT2|STEP_BIT3);
_delay_ms(1);
STEP1_PORT |= (STEP_BIT2|STEP_BIT4);
STEP1_PORT &= ~(STEP_BIT1|STEP_BIT3);
_delay_ms(1);
STEP1_PORT |= (STEP_BIT2|STEP_BIT3);
STEP1_PORT &= ~(STEP_BIT1|STEP_BIT4);
_delay_ms(1);
STEP1_PORT |= (STEP_BIT1|STEP_BIT3);
STEP1_PORT &= ~(STEP_BIT2|STEP_BIT4);
_delay_ms(1);
}
void charForward() /* move char-motor forward by 1 step in 4ms */
{
STEP2_PORT |= (STEP_BIT1|STEP_BIT3);
STEP2_PORT &= ~(STEP_BIT2|STEP_BIT4);
_delay_ms(1);
STEP2_PORT |= (STEP_BIT2|STEP_BIT3);
STEP2_PORT &= ~(STEP_BIT1|STEP_BIT4);
_delay_ms(1);
STEP2_PORT |= (STEP_BIT2|STEP_BIT4);
STEP2_PORT &= ~(STEP_BIT1|STEP_BIT3);
_delay_ms(1);
STEP2_PORT |= (STEP_BIT1|STEP_BIT4);
STEP2_PORT &= ~(STEP_BIT2|STEP_BIT3);
_delay_ms(1);
}
void charBackward() /* move page-motor backward by 1 step in 4ms */
{
STEP2_PORT |= (STEP_BIT1|STEP_BIT4);
STEP2_PORT &= ~(STEP_BIT2|STEP_BIT3);
_delay_ms(1);
STEP2_PORT |= (STEP_BIT2|STEP_BIT4);
STEP2_PORT &= ~(STEP_BIT1|STEP_BIT3);
_delay_ms(1);
STEP2_PORT |= (STEP_BIT2|STEP_BIT3);
STEP2_PORT &= ~(STEP_BIT1|STEP_BIT4);
_delay_ms(1);
STEP2_PORT |= (STEP_BIT1|STEP_BIT3);
STEP2_PORT &= ~(STEP_BIT2|STEP_BIT4);
_delay_ms(1);
}
usbMsgLen_t usbFunctionSetup(uchar data[8]) /* function where all usb-requests are handled */
{
    usbRequest_t    *rq = (void *)data;
    int value;
    value=rq->wValue.bytes[0]; /* for 1 byte transfer, bytes[0] only is required */
    switch(rq->bRequest)
    {
        case PAGE_FWD:
            while(value--) /* duration */
            pageForward();
            break;
        case PAGE_BCK:
            while(value--)
            pageBackward();
            break;
        case CHAR_FWD:
            while(value--)
            charForward();
            break;
        case CHAR_BCK:
            while(value--)
            charBackward();
            break;
        case PEN_DOWN:
            SERVO_PORT |= SERVO_BIT;
            _delay_us(100);
            break;
        case PEN_HOLD:
            SERVO_PORT &= ~(SERVO_BIT);
            _delay_ms(1);
            break;
    }
    return 0;   /* default for not implemented requests: return no data back to host */
}

/* ------------------------------------------------------------------------- */

int __attribute__((noreturn)) main(void)
{
uchar   i;

    wdt_enable(WDTO_1S);
    odDebugInit();
    DBG1(0x00, 0, 0);       /* debug output: main starts */
    usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    i = 0;
    while(--i){             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();
    DIR_PORTB |= (STEP_BIT1|STEP_BIT2|STEP_BIT3|STEP_BIT4);
    DIR_PORTC |= (STEP_BIT1|STEP_BIT2|STEP_BIT3|STEP_BIT4|SERVO_BIT);
    sei();
    DBG1(0x01, 0, 0);       /* debug output: main loop starts */
    for(;;){                /* main event loop */
        DBG1(0x02, 0, 0);   /* debug output: main loop iterates */
        wdt_reset();
        usbPoll();
    }
}
