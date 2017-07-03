#include "sys.h"

void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//set NVIC interrupt groups 2 bits for pre-emption priority, 2 bits for subpriority 
}
