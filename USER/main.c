#include "System.h"
#include "Demo.h"

int main(void)
{ 
	SystemInitialize();
	DemoInit();
	printf("system init ok!\n");
	while(1)
	{
		SystemPoll();
		DemoPoll();
	}
} //end of main 
