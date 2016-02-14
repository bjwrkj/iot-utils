#ifndef _COMMAND_H_
#define _COMMAND_H_
/*HELP*/
#define HELP_MSG "Tl_Commands:\n"\
	"\tdev_cfg [main_module] [sub_module] [key_word] [value]\n"\
	"\t[main_module]:Main module.\n"\
	"\t[sub_module]:Sub module.\n"\
	"\t[key_word]:Operating symbol.\n"\
	"\t\tr:Read value from the address.\n"\
	"\t\tw:Write value to the address.\n"\
	"\t\trd:Read the debug status.\n"\
	"\t\twd:Write the debug status.\n"\
	"\t\tlb:Loopback test.\n"\
	"\t\tst:Get the status.\n"\
	"\t\tt:Test the chip.\n"\
	"\t[offset]:Offset address.Eg:0xXXXXXXXX.\n"\
	"\t[value]:The value written to the address.HEX mod.\n" 
	

#define ERROR_INVALID_MSG "Invalid command line.\n"
#define ERROR_KERNEL_NEXISTS_MSG "Kernel module does not exist.Procedure exits.\n"
#define ERROR_KERNEL_OPERATION_MSG "Kernel module operation error.Procedure exits.\n"
#define ERROR_NENOUGH_MEM_MSG "Not enough memory.Command procedure exits.\n"


#define DEV_NAME "/dev/iotconnect/iot"

#define MAX_DEVICE 512

typedef struct online_devinfo
{
	char vendor_id[16];
	char dev_name[32];
	char serial_number[64];
	char status;
	char count;
}online_devinfo_s;
/*public struct*/
typedef struct iot_dev_info
{
    char vendor_id[16]; /*main module name*/
    char dev_name[32]; /*sub module name*/
    int key;		/*key*/
    unsigned int argc;		/*argc*/
    char **argv;
    unsigned long argl[16];
	online_devinfo_s current_online_devinfo[MAX_DEVICE];
}iot_dev_info_s;

typedef struct dev_op_keyword
{
	signed char  key_word[64];/*key*/
	signed int key_num;/*num*/
}dev_op_keyword_t;

/*enum key*/
enum cmd
{
	CMD_LIST_MAIN = -3,
	CMD_LIST_SUB = -2,
	CMD_CUSTOM = -1,
	CMD_LIST = 0,
	CMD_SCAN,
	CMD_ALL_STATE,
	CMD_HELP,
	CMD_OPEN,
	CMD_CLOSE,
	CMD_STATE,
	CMD_DEVICE_TEST,
	CMD_DEVICE_RST,
	CMD_KEYWORD_LENDTH

};
enum op_type
{
	OPEN = 0,
	CLOSE,
	SCAN, 
	LIST,
	TEST,
	RESET,
	OP_LENGTH
};
typedef enum{
	BOOL_FALSE,
	BOOL_TRUE
}bool_t;

#define CMD_KEYWORD_MAXBYTE 256 /*input max keyword length*/
#ifndef VERSION
#define VERSION "1.0"
#endif

#endif
