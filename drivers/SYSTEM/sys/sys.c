#include "sys.h"

void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
void MSR_MSP(uint32_t addr)
{
    __asm volatile ("MSR MSP, %0" : : "r" (addr) : );
}
