
#ifndef IOT_CORE_H_
#define IOT_CORE_H_
#include <linux/module.h>  
#include <linux/init.h>    	/* module_init() */
#include <linux/delay.h>	
#include <linux/irq.h>
#include <linux/interrupt.h> 	/* interrupt */

#include <linux/kernel.h> 	/* printk() */
#include <linux/slab.h> 	/* kmalloc() */
#include <linux/fs.h>		/* everything ..*/
#include <linux/errno.h> 	/* error number codes */
#include <linux/types.h>	/* size_t */
#include <linux/cdev.h>
#include <linux/device.h>	/* class_create */

#include <asm/uaccess.h>	/* copy_*_user */
#include <linux/vmalloc.h>

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
typedef enum
{
	BOOL_FALSE,
 	BOOL_TRUE
}bool_t;
enum
{
	RET_OK = 0,
	RET_ERR ,
	RET_NOTSUPPORT ,
	RET_FULL 
};

#define CMD_KEYWORD_MAXBYTE 256 /* 用户输入的关键字最大字节数 */
#define MAX_DEVICE 512

typedef struct dev_op_keyword
{
	signed char  key_word[32];/*key word*/
	signed int key_num;/*key num*/
}dev_op_keyword_t;
typedef struct iotdev
{
	const struct iot_dev_ops *ops;
	unsigned int count;
};

typedef  struct iot_dev_ops
{
	unsigned char vendor_id [32];/*vendor id*/
	unsigned char dev_name [32];/*devices name*/
	unsigned int *private_ptr;/*private*/
	/*scaning device*/
	unsigned int (*scan)(const struct iot_dev_ops *dev,char *value);
	/*open and close device*/
	unsigned short (*open_close)(const struct iot_dev_ops *dev,
			    			 const unsigned char type,
		            		 char *value);
	unsigned short (*custom)(const struct iot_dev_ops *dev,				
				 const unsigned int argc,
				 unsigned char ** argv,
				 unsigned char *ret_msg);
	unsigned short (*state)(const struct iot_dev_ops *dev);
	unsigned int (*chip_test)(const struct iot_dev_ops *dev);
	unsigned short(*rst)(struct iot_dev_ops *dev,char *retmsg);
}iot_device_s;
typedef struct link_node
{
	unsigned char *data;
	struct link_node * link;
}linklist_t;
typedef struct online_devinfo
{
	char vendor_id[16];
	char dev_name[32];
	char serial_number[64];
	char status;
	char count;
}online_devinfo_s;
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

typedef struct iot_dev_node
{
	iot_device_s *data;
//	struct iot_dev_node *next;
}iot_support_dev_list_s;
/*Vendor ID*/
#define SAMSUANG           "samsuang"
#define NXP				   "nxp"
/*Device_ID*/
#define FS2416				"fs2416"

struct iot_dev_ops* iot_search_devlist(struct iot_dev_ops *dev);
int iot_iscmd_devlist(char* strCmd, dev_op_keyword_t *pst_cmd, int cmd_len);
struct iot_dev_ops* iot_search_devlist(struct iot_dev_ops *dev);
int iot_register_devlist(iot_device_s *dev);
void iot_init(struct iotdev *dev,struct iot_dev_ops *ops);


#endif 
