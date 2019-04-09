#include <string.h>
#include "cmdline.h"

#include "main.h"

int (*rx_ReadByte)(uint8_t timeout) = USARTn_Rx_ReadByte;


extern uint8_t __tx_buffer[Buffer_Size];
extern uint8_t __rx_buffer[Buffer_Size];
extern uint16_t __rx_buffer_head, __rx_buffer_tail;
uint16_t get_cmd_buffer(uint8_t idx, char *buff, uint16_t len)
{
	char* p = buff;
	int ch;
	while (p - buff < len -1)
	{
		ch = rx_ReadByte(0);
		while(ch == 0x7e)
		{
			if((ch = rx_ReadByte(0)) == 0x7e)
			{
				continue;
			}
			else
			{
				*p++ = 0x7e;
				*p++ = ch;
				ch = rx_ReadByte(0);
				break;
			}
		}
		// while(ch == '*')
		// {
		// 	ch = rx_ReadByte(idx, 0);
		// }
		if(ch >= 0) {
			if(0x7f == ch)
			{
				*p++ = ch;
				break;
			}
			// if('#' == ch)
			// {
			// 	break;
			// }
			else {
			*p ++ = ch;
			}
		}
		 else{
		 	return 0;
		 }
	}
	*p = 0;
	return p-buff;
}

void dispatch (struct cmdline* cmd)
{
	int i;
        //hal_debug("d\r\n");
	for (i = 0; i < countof(cmds); i++) {
		// if (cmds[i].name == cmd->params[0]) {
		if(cmds[i].cmdid == cmd->cmdid){
		// if(cmds[i].name == cmd->params[0]) || (cmds[i].cmdid == cmd->cmdid))
			cmds[i].proc(cmd);
			return;
		}
	}
}