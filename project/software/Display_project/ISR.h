#include "sys/alt_timestamp.h"
#include "sys/alt_alarm.h"
#include "sys/alt_irq.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_timer_regs.h"

int curr_room_num;

void timer_0_ISR(void* context, alt_u32 id)
{
	//printf("Starting Interruppt\n");
   // clear irq status in order to prevent retriggering
   IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);

   //sendAndGetTemp();

   //printf("Finished Interrupt\n");

}

void Init_ISR(void){
	 printf("INIT ISR \n");

	 //Timer Initialization
	  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0x0003);
	  IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);

	  IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_0_BASE, 0x9f00);
	  IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_0_BASE, 0x5000);


	  //Register ISR for timer event
	  alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, timer_0_ISR, NULL, 0);

	  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0x0007);
}

void sendAndGetTemp (void){
	sendTempRequest();
	printf("Getting temp\n");
	while (getcharRS232!='t'){}
	int tens = getcharRS232()-48;
	printf("tens is %d\n",tens);
	int ones = getcharRS232()-48;
	printf("ones is %d\n",ones);
	printf("temp is %d\n",10*tens+ones);
	roomArray[curr_room_num-1].temp = 10*tens+ones;
}
