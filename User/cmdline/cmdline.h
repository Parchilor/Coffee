#ifndef __CMDLINE_H_
#define __CMDLINE_H_
#include "cmdfuncs.h"

// const static struct {
// 	char* name;
// 	void (*proc) (struct cmdline*);
// 	char* help_msg;
// } cmds[] = {
// 	{"help", cmd_help, "Show All Commands"},
// 	{"reset", cmd_reset, "reset cpu"},
// 	{"reboot", cmd_reboot, "reboot"},
// };

const static struct {
	char *name;
	uint16_t cmdid;
	void (*proc)(struct cmdline *);
	char *help_msg;
} cmds[] = {
	// {"motor_dir", 0xBBBB, cmd_motor_dir, "Set Motor Dir"},
	{"sys_state", 0xCCCC, cmd_system_state, "Get System State"},
	{"coil", 0x5555, cmd_coil, "Coil Control"},
	{"motor", 0xAAAA, cmd_motor, "Motor Control"},
	{"reset", 0xFFFF, cmd_reset, "Reset the Soc"},
	{"reboot", 0xEEEE, cmd_reboot, "Reboot the Soc"},
	{"help", 0xDDDD, cmd_help, "Help"},

};
uint16_t get_cmd_buffer(uint8_t idx, char *buff, uint16_t len);
//uint16_t get_cmd_buffer(uint8_t idx, char *buff, uint16_t len, uint8_t mode);
void dispatch (struct cmdline* cmd);
#endif