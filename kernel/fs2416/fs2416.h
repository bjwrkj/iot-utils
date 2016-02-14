#ifndef __FS2146_H__
#define __FS2146_H__

enum fs2416_keyword_s
{
	FLASH_CMD_HELP 	=0,
	FLASH_UPDATE,
	FLASH_GET_MAIN_FILE,
	FLASH_GET_BACK_FILE,
	FLASH_SET_MAIN_FILE,
	FLASH_SET_BACK_FILE,
    OP_MAX_NUM
};
unsigned short fs2416_init(void);
#endif /*__FS2146_H__ */


