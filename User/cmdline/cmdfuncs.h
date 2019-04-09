#ifndef __CMDFUNCS_H_
#define __CMDFUNCS_H_
#include <stdint.h>
#define MAX_CMD_LENGTH 32
#define MAX_CMD_ITEM   6
// struct cmdline {
// 	int count;
// 	const char* params[MAX_CMD_ITEM];
// };

struct cmdline {
	uint8_t idx;
	uint16_t cmdid;
	uint16_t len;
	uint8_t data[32];
};
void cmd_help(struct cmdline *cmd);
void cmd_reset(struct cmdline *cmd);
void cmd_reboot(struct cmdline *cmd);
void cmd_coil(struct cmdline *cmd);
void cmd_motor(struct cmdline *cmd);
void cmd_system_state(struct cmdline *cmd);
void cmd_motor_dir(struct cmdline *cmd);

#endif