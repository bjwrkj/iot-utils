
#include "../iot_core.h"
#include "fs2416.h"

static dev_op_keyword_t fs2416_keyword[OP_MAX_NUM] 
                                    = {{.key_word = "help",
										.key_num = FLASH_CMD_HELP},
									   {.key_word = "update",
										.key_num = FLASH_UPDATE},										
									   {.key_word = "get_main",
										.key_num = FLASH_GET_MAIN_FILE},										
									   {.key_word = "get_back",
										.key_num = FLASH_GET_BACK_FILE},
									   {.key_word = "set_main",
										.key_num = FLASH_SET_MAIN_FILE},
									   {.key_word = "set_back",
										.key_num = FLASH_SET_BACK_FILE}
                                  };

unsigned int fs2416_scan(const struct iot_dev_ops *dev,char *value)
{
	int retval = RET_OK;
	printk("Scaning fs2416 OK\n");
	*value = 1;
	return retval;
}
unsigned short fs2416_open_close(const struct iot_dev_ops *dev,
			    			 const unsigned char type,
		            		 unsigned char *value)
{
	int retval = RET_OK;
	return retval;
}
unsigned int fs2416_test(const struct iot_dev_ops *dev)
{
	int retval = RET_OK;
	return retval;
}
unsigned short fs2416_state(const struct iot_dev_ops *dev)
{
	int retval = RET_OK;
	return retval;
}
unsigned short fs2416_reset(struct iot_dev_ops *dev,char *retmsg)
{
	int retval = RET_OK;
	return retval;
}
unsigned short fs2416_coustom(const struct iot_dev_ops *dev,unsigned int argc,unsigned char ** argv,unsigned char *ret_msg)
{    
	unsigned short ret = 0;
	unsigned int key_cmd ;
	unsigned int reg_value = 0;
	if (0 == argc)
    {
//        strcpy((char *)ret_msg,(char *)OP_HELP);
        return RET_ERR;
    }

    key_cmd = iot_iscmd_devlist(argv[0], fs2416_keyword, OP_MAX_NUM);
	argv++;
	argc--;
	switch(key_cmd)
	{
		default:
//			strcpy((char *)ret_msg,(char *)OP_HELP);
			break;
	}

}
static const struct iot_dev_ops fs2416_ops = {
	.vendor_id	= SAMSUANG,
	.dev_name   = FS2416,
	.scan		= fs2416_scan,
	.open_close 	= fs2416_open_close,
	.state		= fs2416_state,
	.chip_test	= fs2416_test,
	.rst		= fs2416_reset,
	.custom	= fs2416_coustom,
};

unsigned short fs2416_init(void)
{
    struct iotdev *fs2416_dev;
	bool_t bool = BOOL_FALSE;
	fs2416_dev = (struct iotdev *)kmalloc(sizeof(struct iotdev),GFP_KERNEL);
	iot_init(&fs2416_dev,&fs2416_ops);
	if(fs2416_dev == NULL)
	{
		return RET_ERR;
	}
	bool = iot_register_devlist(&fs2416_dev->ops);
	if(bool == BOOL_FALSE)
	{
		printk("register device faild.\n");
        kfree(fs2416_dev);
		return RET_ERR;
	}
    return RET_OK;
}
