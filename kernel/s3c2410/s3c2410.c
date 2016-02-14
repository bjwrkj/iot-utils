
#include "../iot_core.h"
#include "s3c2410.h"

static dev_op_keyword_t s3c2410_keyword[S3C2410_OP_MAX_NUM] 
                                    = {{.key_word = "help",
										.key_num = S3C2410_HELP},
                                  };

unsigned int s3c2410_scan(const struct iot_dev_ops *dev,char *value)
{
	int retval = RET_OK;
	printk("Scaning s3c2410 OK\n");
	*value = 1;
	return retval;
}
unsigned short s3c2410_open_close(const struct iot_dev_ops *dev,
			    			 const unsigned char type,
		            		 unsigned char *value)
{
	int retval = RET_OK;
	return retval;
}
unsigned int s3c2410_test(const struct iot_dev_ops *dev)
{
	int retval = RET_OK;
	return retval;
}
unsigned short s3c2410_state(const struct iot_dev_ops *dev)
{
	int retval = RET_OK;
	return retval;
}
unsigned short s3c2410_reset(struct iot_dev_ops *dev,char *retmsg)
{
	int retval = RET_OK;
	return retval;
}
unsigned short s3c2410_coustom(const struct iot_dev_ops *dev,unsigned int argc,unsigned char ** argv,unsigned char *ret_msg)
{    
	unsigned short ret = 0;
	unsigned int key_cmd ;
	unsigned int reg_value = 0;
	if (0 == argc)
    {
//        strcpy((char *)ret_msg,(char *)OP_HELP);
        return RET_ERR;
    }

    key_cmd = iot_iscmd_devlist(argv[0], s3c2410_keyword, S3C2410_OP_MAX_NUM);
	argv++;
	argc--;
	switch(key_cmd)
	{
		default:
//			strcpy((char *)ret_msg,(char *)OP_HELP);
			break;
	}

}
static const struct iot_dev_ops s3c2410_ops = {
	.vendor_id	= SAMSUANG,
	.dev_name   = "s3c2410",
	.scan		= s3c2410_scan,
	.open_close 	= s3c2410_open_close,
	.state		= s3c2410_state,
	.chip_test	= s3c2410_test,
	.rst		= s3c2410_reset,
	.custom	= s3c2410_coustom,
};

unsigned short s3c2410_init(void)
{
    struct iotdev *s3c2410_dev;
	bool_t bool = BOOL_FALSE;
	s3c2410_dev = (struct iotdev *)kmalloc(sizeof(struct iotdev),GFP_KERNEL);
	iot_init(&s3c2410_dev,&s3c2410_ops);
	if(s3c2410_dev == NULL)
	{
		return RET_ERR;
	}
	bool = iot_register_devlist(&s3c2410_dev->ops);
	if(bool == BOOL_FALSE)
	{
		printk("register device faild.\n");
        kfree(s3c2410_dev);
		return RET_ERR;
	}
    return RET_OK;
}
