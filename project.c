#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>

#define DELAY 304000
#define MCLK_FREQUENCY 3000000
#define PERIOD (MCLK_FREQUENCY/46)
int i;
int left=0;
int right=0;

const uint8_t port_mapping[] =
{
 PM_NONE,PM_NONE,PM_NONE,PM_NONE,PM_NONE,PM_NONE,PM_TA0CCR1A,PM_TA0CCR2A
};

void main(void) {
    WDT_A_holdTimer();

    //switches
    P1->DIR &= ((~BIT1|BIT4));
    P1->DIR |= BIT0;
    P1->REN = BIT1|BIT4;
    P1->OUT = BIT1|BIT4;

    //motor
    GPIO_setAsOutputPin(GPIO_PORT_P5,GPIO_PIN4|GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P3,GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN6|GPIO_PIN7);
    GPIO_setOutputHighOnPin(GPIO_PORT_P5,GPIO_PIN4|GPIO_PIN5);

    //Port2
    PMAP_configurePorts((const uint8_t *) port_mapping,PMAP_P2MAP,1,PMAP_DISABLE_RECONFIGURATION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2,GPIO_PIN6|GPIO_PIN7,GPIO_PRIMARY_MODULE_FUNCTION);

    //TimerA0
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK|TIMER_A_CTL_MC__UP|TIMER_A_CTL_CLR;
    TIMER_A0->CCR[0] = PERIOD;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCR[1] = left;
    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCR[2] = right;

    //IRLED, (unneeded)
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P9, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN3);
    GPIO_setOutputLowOnPin(GPIO_PORT_P9, GPIO_PIN2);

    //main process
    while(1){
        if((P1->IN&BIT1)==0X00) { //CLOCKWISE
            for(i=0;i<750000;i++){} //3s

            //forward
            left = PERIOD;
            right = PERIOD;
            TIMER_A0->CCR[1] = left;
            TIMER_A0->CCR[2] = right;

            for(i=0;i<250000;i++){}

            //turn right
            left = PERIOD/5;
            right = 0;
            TIMER_A0->CCR[1] = left;
            TIMER_A0->CCR[2] = right;

            for(i=0;i<222000;i++){} //90 deg turn

            //forward
            left = PERIOD;
            right = PERIOD;
            TIMER_A0->CCR[1] = left;
            TIMER_A0->CCR[2] = right;

            for(i=0;i<50000;i++){}

            //turn right
            left = PERIOD/5;
            right = 0;
            TIMER_A0->CCR[1] = left;
            TIMER_A0->CCR[2] = right;

            for(i=0;i<222000;i++){} //90 deg turn

            //forward
            left = PERIOD;
            right = PERIOD;
            TIMER_A0->CCR[1] = left;
            TIMER_A0->CCR[2] = right;

            for(i=0;i<250000;i++){}

            //STOP
            left = 0;
            right = 0;
            TIMER_A0->CCR[1] = left;
            TIMER_A0->CCR[2] = right;
        }
        if((P1->IN&BIT4)==0X00) { //C-CLOCKWISE
            for(i=0;i<600000;i++){}//2s

            //forward
            left = PERIOD;
            right = PERIOD;
            TIMER_A0->CCR[1] = left;
            TIMER_A0->CCR[2] = right;

            for(i=0;i<250000;i++){}

            //turn LEFT
            left = 0;
            right = PERIOD/5;
            TIMER_A0->CCR[1] = left;
            TIMER_A0->CCR[2] = right;

            for(i=0;i<220000;i++){} //90 deg turn

            //forward
            left = PERIOD;
            right = PERIOD;
            TIMER_A0->CCR[1] = left;
            TIMER_A0->CCR[2] = right;

            for(i=0;i<50000;i++){}

            //turn left
            \
            left = 0;
            right = PERIOD/5;
            TIMER_A0->CCR[1] = left;
            TIMER_A0->CCR[2] = right;

            for(i=0;i<220000;i++){} //90 deg turn

            //forward
            left = PERIOD;
            right = PERIOD;
            TIMER_A0->CCR[1] = left;
            TIMER_A0->CCR[2] = right;

            for(i=0;i<250000;i++){}

            //STOP
            left = 0;
            right = 0;
            TIMER_A0->CCR[1] = left;
            TIMER_A0->CCR[2] = right;
        }
}
}
