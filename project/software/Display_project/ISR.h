#include "sys/alt_timestamp.h"
#include "sys/alt_alarm.h"
#include "sys/alt_irq.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_timer_regs.h"

int curr_room_num;

void timer_0_ISR(void* context, alt_u32 id)
{
	printf("\nStarting Interruppt\n");
   // clear irq status in order to prevent retriggering
   IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);

   	SendRangeRequest();
   	sendTempRequest();

   printf("Finished Interrupt\n\n");

}

void Init_ISR(void){
	printf("INIT ISR \n");

	//Timer Initialization
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0x0003);
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);

	//Period = X second * 50000000 
	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_0_BASE, 0x3000);
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_0_BASE, 0x3000);

	//Register ISR for timer event
	alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, timer_0_ISR, NULL, 0);

	//Start timer
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0x0007);
}

void sendAndGetTemp (void){

	/*printf("Getting temp\n");
	while (getcharRS232()!='t'){}
	int tens = getcharRS232()-48;
	//printf("tens is %d\n",tens);
	int ones = getcharRS232()-48;
	//printf("ones is %d\n",ones);
	printf("Temp is %d\n",10*tens+ones);
	roomArray[curr_room_num-1].temp = 10*tens+ones;

	//roomArray[curr_room_num-1].lights = sendLightRequest();

	InfoDisplay(curr_room_num, roomArray[curr_room_num - 1].lights,roomArray[curr_room_num - 1].door,roomArray[curr_room_num - 1].occupied,roomArray[curr_room_num - 1].in_use, roomArray[curr_room_num - 1].temp);
	int k;
	for (k = 0; k < 10; k++) {
		if (roomArray[k].requested == 1 && roomArray[k].in_use==1) {
			RequestCloseDisplay(k +1);
		}
		else if (roomArray[k].requested == 1 && roomArray[k].in_use==0) {
			RequestOpenDisplay(k +1);
		}
	}
	PrintNumbers(curr_room_num);*/
}
