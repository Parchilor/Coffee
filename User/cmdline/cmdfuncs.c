#include "cmdfuncs.h"

#include "main.h"
#include <string.h>

void cmd_help(struct cmdline *cmd)
{
	int i;
	for (i = 0; i < countof(cmds); i++)
	{
		// if(cmds[i].help_msg == 0) continue;
		hal_debug("0x%X : %s\r\n", cmds[i].name, cmds[i].help_msg);
	}
}

void cmd_reset(struct cmdline *cmd)
{
	__cli();
	NVIC_SystemReset();
}

void cmd_reboot(struct cmdline *cmd)
{
	__cli();
	// Save data to inner flash

	NVIC_SystemReset();
}
