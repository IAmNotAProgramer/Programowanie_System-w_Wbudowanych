#include <LPC21xx.H>
#include "led.h"

// TIMER
#define mCOUNTER_ENABLE 0x00000001
#define mCOUNTER_RESET  0x00000002

// CompareMatch
#define mINTERRUPT_ON_MR0 0x00000001
#define mRESET_ON_MR0     0x00000002
#define mMR0_INTERRUPT    0x00000001

// VIC (Vector Interrupt Controller) VICIntEnable
#define VIC_TIMER1_CHANNEL_NR 5
#define VIC_TIMER0_CHANNEL_NR 4
void (*ptrTimer0InterruptFunction)();
// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE 0x00000020 //aktywacja slot irq tylko bit 5

/**********************************************/
//(Interrupt Service Routine) of Timer 0 interrupt
__irq void Timer0IRQHandler(){

	T0IR=mMR0_INTERRUPT; 	// skasowanie flagi przerwania 
	ptrTimer0InterruptFunction();		// cos do roboty
	VICVectAddr=0x00; 	// potwierdzenie wykonania procedury obslugi przerwania
}
/**********************************************/
void Timer0Interrupts_Init(unsigned int uiPeriod,void (*ptrInterruptFunction)()){ // microseconds

        // interrupts
	
	ptrTimer0InterruptFunction=ptrInterruptFunction;
	
	VICIntEnable |= (0x1 << VIC_TIMER0_CHANNEL_NR);            // Enable Timer 0 interrupt channel 
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_TIMER0_CHANNEL_NR;  // Enable Slot 0 and assign it to Timer 0 interrupt channel
	VICVectAddr1  =(unsigned long)Timer0IRQHandler; 	   // Set to Slot 0 Address of Interrupt Service Routine 

        // match module

	T0MR0 = 15000 * uiPeriod;                 	      // value 
	T0MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); // action 

        // timer

	T0TCR |=  mCOUNTER_ENABLE; // start 

}
/**********************************************/
int main (){
	unsigned int iMainLoopCtr;
//	void (*wsk_funckja)();
	//wsk_funckja=&Led_StepRight;
	Timer0Interrupts_Init(250,&Led_StepLeft);

	while(1){
	 	iMainLoopCtr++;
	}
}
