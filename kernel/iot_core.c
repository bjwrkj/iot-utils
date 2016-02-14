/* driver:Character driver
 * auther:Keven_Ongoingcre
 * date:2015-12-9
 * details:ONGOINGCRE Smart L General Character driver 
 *
 */
#include "iot_debug.h"
#include "iot_core.h"
#include "fs2416/fs2416.h"
#include "s3c2410/s3c2410.h"
#define ONGOINGCRE_MAJOR 249
#define N 64
static int iot_major;// = ONGOINGCRE_MAJOR;
static int iot_dev_info_count = 1;
static int iot_open_dev_count = 0;
static int  iot_current_dev_count;
iot_dev_info_s *iot_return;
iot_dev_info_s *iot_copy;
static int global_dev_number = 0;

static struct cdev_dev
{
//	struct semaphore sem;
	struct cdev cdev;
};
struct cdev_dev *iot_cdev;
static struct class *iot_char_class;
/*防止多次打开文件*/
unsigned int g_iot_dev_inc = 0;
char online_vendor_id[256];

/*设备结构体链表*/
struct iot_dev_node *iot_support_dev_list;
void iot_linklist_distruct(linklist_t *list)
{
	linklist_t *p,*r;
	p = list;
	while(p!=NULL)
	{
		r=p;
		p=p->link;
		kfree(r);
	}
    
	return;
}
int iot_linklist_print(linklist_t* list)
{
	linklist_t *p;
	int counter;

	counter = 0;
	p=list;
	while(p!=NULL)
	{
		strcpy(iot_return->current_online_devinfo[counter].vendor_id,p->data);
		p=p->link;
		counter++;
	}
    
	return counter;
}

linklist_t* iot_linklist_contains(linklist_t  *list,u8 * main_name)
{
	linklist_t *p;

	p = list;
	while(p!=NULL)
	{
		if(strcasecmp((char *)p->data, (char *)main_name)==0)
		{
			return p;
		}
		p=p->link;
	}
    
	return NULL;
}
void iot_linklist_insert(linklist_t ** list,u8 * main_name)
{
	linklist_t *p,*r;

	if(iot_linklist_contains(*list,main_name)!=NULL )
	{
		return ;
	}
	p = NULL;
	r = NULL;
	if(*list == NULL)
	{
		*list=(linklist_t *)kmalloc(sizeof(linklist_t),GFP_KERNEL);
		if(*list == NULL)
		{
			return;
		}
		(*list)->data = main_name;
		(*list)->link = NULL;
	}
	else
	{
		p = *list;
		
		while(p!=NULL)
		{
			r = p;
			p = p->link;
		}
		p=(linklist_t *)kmalloc(sizeof(linklist_t),GFP_KERNEL);
		if(p==NULL)
		{
			return ;
		}
		p->data = main_name;
		p->link = NULL;
		if(NULL == r)
		{
		    kfree(p);
			return ;
		}
		r->link = p;
	}

	return;
}

int iot_print_vendor_list(void)
{
	struct iot_dev_node *p ;
	int dev_count;
	linklist_t *main_list;
	int i;
	
	p = iot_support_dev_list;
	if(p == NULL)
	{
		pr_alert("\nThere is no device online connected.\n");
		return -1;
	}
	main_list = NULL;	
	for(i = 0;i < iot_current_dev_count; i++)
	{
		iot_linklist_insert(&main_list,p->data->vendor_id);
		p++;
	}
	dev_count = iot_linklist_print(main_list);
	iot_linklist_distruct(main_list);

    return dev_count;
}
int iot_print_devname_list(const char* vendor_id)
{
	int i, dev_count;
	iot_support_dev_list_s *p = iot_support_dev_list;
	if(p == NULL)
	{
		pr_alert("\nThere is no device online connected.\n");
		return -1;
	}
	dev_count=0;
	for(i = 0; i < iot_current_dev_count; i++)
	{
		if(strcasecmp((char *)p->data->vendor_id, (char *)vendor_id)==0)
		{
			strcpy(iot_copy->current_online_devinfo[dev_count].vendor_id,(char *)p->data->vendor_id);
			strcpy(iot_copy->current_online_devinfo[dev_count].dev_name,(char *)p->data->dev_name);	
			dev_count++;
		}
		p++;
	}
	if(dev_count==0)
	{
		pr_alert("There're no sub devices of module[%s].\n",vendor_id);
	}
    else
	{
		pr_info("Total sub device number is [%d].\n",dev_count);
	}
	
	return dev_count;
}

int iot_iscmd_devlist(char* strCmd, dev_op_keyword_t *pst_cmd, int cmd_len)
{
	int i;
	dev_op_keyword_t *pstcmd_tmp = pst_cmd;
	for(i = 0;i < cmd_len; i++)
	{
		pstcmd_tmp = pst_cmd+i;
		if(	pstcmd_tmp != NULL && strcasecmp((int *)strCmd, pstcmd_tmp->key_word) == 0)
		{
			return pstcmd_tmp->key_num;
		}
	}
	return -1;
}
iot_device_s* iot_del_devlist(struct iot_dev_ops *dev)
{
	iot_device_s *dev_tmp;
	iot_support_dev_list_s *p ;
    int i;
	iot_device_s *data;
	dev_tmp = iot_search_devlist(dev);
	if(dev_tmp==NULL){
		return NULL;
	}
	p = iot_support_dev_list;
	for(i = 0; i < iot_current_dev_count; i++)
	{
		data = p->data;
		if (dev_tmp == data){
			break;
		}
		p++;
	}
	iot_current_dev_count--;	
	for( ;i < iot_current_dev_count; i++)
	{
		p->data = (p + 1)->data;
		p++;
	}
	return dev_tmp;
}

struct iot_dev_ops* iot_search_devlist(struct iot_dev_ops *dev)
{
	iot_support_dev_list_s *p;
	s32 mnflag;
	s32 snflag;
	iot_device_s *data;
	u32 i;
	
	p = iot_support_dev_list;
	if(dev == NULL )
	{
		return NULL;
	}

	for(i = 0;i < iot_current_dev_count;i++)
	{
		data = p->data;
		if ( NULL == data )
		{
			break;
		}
		mnflag = strcasecmp((char *)dev->vendor_id, (char *)data->vendor_id);
		snflag = strcasecmp((char *)dev->dev_name, (char *)data->dev_name);
		if((mnflag==0)&&(snflag==0))
		{
			return p->data;
		}
		p++;
	}
	return NULL;
}
/*iot core register support*/
void iot_init(struct iotdev *dev,struct iot_dev_ops *ops)
{
	memset(dev, 0, sizeof *dev);
	dev->ops = ops;
}

int iot_register_devlist(iot_device_s *dev)
{
 	iot_support_dev_list_s *p;
 	if(NULL == dev || NULL != iot_search_devlist(dev))
	{
        pr_devel("vendor_id=%s,dev_name=%s\r\n",dev->vendor_id,dev->dev_name);
 		return BOOL_FALSE;
	}
	p = iot_support_dev_list + iot_current_dev_count;
 	p->data = dev;	
 	iot_current_dev_count++;
	pr_info("register devices [%s],[%s]\n",dev->vendor_id,dev->dev_name);
 	return BOOL_TRUE;
}

static int iot_open(struct inode *inode,struct file *flip)
{
	struct cdev_dev *dev;	
	if(iot_open_dev_count > 0) 
		return -ERESTARTSYS;
	iot_open_dev_count++;
	dev = container_of(inode->i_cdev,struct cdev_dev,cdev);
	flip->private_data = dev;
	return 0;
}
static int iot_release(struct inode *inode,struct file *filp)
{
	iot_open_dev_count--;
	return 0;
}
static ssize_t iot_read(struct file *filp,char __user *buff,size_t size,loff_t *ppos)
{
	ssize_t retval = BOOL_TRUE;
	int vendor_number,dev_number = 0,i,j;
	iot_dev_info_s *iot_send;
	struct iot_dev_ops dev_key; 
	struct iot_dev_ops* cmd_dev;
	char status;
	iot_send=(iot_dev_info_s *)kmalloc(sizeof(iot_dev_info_s),GFP_KERNEL);
	iot_return = (iot_dev_info_s *)kmalloc(sizeof(iot_dev_info_s),GFP_KERNEL);
	iot_copy = (iot_dev_info_s *)kmalloc(sizeof(iot_dev_info_s),GFP_KERNEL);
    if(NULL == iot_send)
    {
        pr_alert("malloc iot_send error\n");
		retval = BOOL_FALSE;
		goto err;
    }
	if(copy_from_user(iot_send, buff, (int)size)) 
	{
        kfree(iot_send);
        pr_alert("copy_from_user error\n");
		retval = BOOL_FALSE;
		goto err;
	}
	switch(iot_send->key)
    {
        case SCAN:
        {
			vendor_number = iot_print_vendor_list();
			for (i = 0; i < vendor_number; i++){
					global_dev_number += iot_print_devname_list(iot_return->current_online_devinfo[i].vendor_id);					
				}
			for (i = 0; i < global_dev_number; i++)
				{					
					strcpy((char *)dev_key.vendor_id, (char *)iot_copy->current_online_devinfo[i].vendor_id);
					strcpy((char *)dev_key.dev_name, (char *)iot_copy->current_online_devinfo[i].dev_name);		
					strcpy(iot_copy->current_online_devinfo[i].serial_number,"4f44e768-a22e-11e5-9005-080027705ae0");	
					cmd_dev = iot_search_devlist(&dev_key);
					if(NULL == cmd_dev){
				        pr_alert("No such device [%s,%s].Exit kernel.\n",iot_copy->current_online_devinfo[i].vendor_id,iot_copy->current_online_devinfo[i].dev_name);		
						return -1;
					}
					cmd_dev->scan(cmd_dev,&status);
					iot_copy->current_online_devinfo[i].status = status;					
					iot_copy->current_online_devinfo[i].count = 0;
				}
			iot_copy->current_online_devinfo[0].count = global_dev_number;
			global_dev_number = 0;
			copy_to_user(buff,(char *)iot_copy,sizeof(iot_dev_info_s));
            break;
        }
        case LIST:
        {
            retval = BOOL_TRUE; 
			break;
        }
        default:
            break;
    }
	kfree(iot_send);
	kfree(iot_return);
	kfree(iot_copy);
	return retval;
err:	
	kfree(iot_send);
	kfree(iot_return);
	kfree(iot_copy);
	return retval;
}
static ssize_t iot_write(struct file *flip,const char __user *buf,size_t count,loff_t *ppos)
{
	ssize_t retval = 0;
	return retval;
}
static ssize_t iot_aio_read(struct kiocb * kiocb,const struct iovec *iovec,unsigned long size,loff_t ppos)
{
	ssize_t retval = 0;
	return retval;
}
static ssize_t iot_aio_write(struct kiocb * kiocb,const struct iovec *iovec,unsigned long size,loff_t ppos)
{
	ssize_t retval = 0;
	return retval;
}
static long iot_unlocked_ioctl(struct file *flip,unsigned int command,unsigned long arg)
{
	return 0;
}
static long iot_compat_ioctl(struct file *flip,unsigned int command,unsigned long arg)
{
	return 0;
}
static const struct file_operations iot_fops = {
	.read		= iot_read,
	.aio_read 	= iot_aio_read,
	.write		= iot_write,
	.aio_write	= iot_aio_write,
	.open		= iot_open,
	.release	= iot_release,
	.unlocked_ioctl	= iot_unlocked_ioctl,
	.compat_ioctl = iot_compat_ioctl,
	.owner		= THIS_MODULE,
};
static char *iot_char_devnode(struct device *dev,umode_t *mode)
{
	return kasprintf(GFP_KERNEL, "iotconnect/%s", dev_name(dev));
}
static void __exit iot_character_cleanup(void)
{
	printk("Oogoingcre IOT exit\n");
	vfree(iot_support_dev_list);
	device_destroy(iot_char_class,MKDEV(iot_major,0));
	class_destroy(iot_char_class);
	cdev_del(&iot_cdev->cdev);  
	unregister_chrdev_region(MKDEV(iot_major, 0), iot_dev_info_count); 
	return;
}
static int __init iot_character_init(void)
{
	int ret;
	unsigned int size;
	dev_t dev = MKDEV(ONGOINGCRE_MAJOR,0);
	printk("Oogoingcre IOT V1.0\n");
	if(iot_major)
		ret = register_chrdev_region(dev,iot_dev_info_count,"iotconnect");
	else
	{
		ret = alloc_chrdev_region(&dev,0,1,"iotconnect");
		iot_major = MAJOR(dev);
	}
	if (ret)
	{
		printk("can't get major = %d\n",iot_major);
		goto err;
	}
	iot_cdev = (struct cdev_dev*)kmalloc(sizeof(struct cdev_dev),GFP_KERNEL);
	if (!iot_cdev) {
		ret = -ENOMEM;		
		goto err;  /* Make this more graceful */
	}
	cdev_init(&iot_cdev->cdev,&iot_fops);	
	ret = cdev_add(&iot_cdev->cdev,dev,iot_dev_info_count);
	if(ret)
		goto err_region;
	/*class_create*/
	iot_char_class = class_create(THIS_MODULE,"IOT_CORE_H_");
	if(IS_ERR(iot_char_class))
	{
		printk(KERN_ERR"Error create iot_character class ....\n");
		cdev_del(&iot_cdev->cdev);
		ret = PTR_ERR(iot_char_class);
		goto err_region;
	}	
	iot_char_class->devnode = iot_char_devnode;
	device_create(iot_char_class, NULL, MKDEV(iot_major, 0), NULL, "iot");
	/*Assigned Device Space*/
	iot_support_dev_list = (iot_support_dev_list_s *)vmalloc(MAX_DEVICE * sizeof(iot_support_dev_list_s));
	if(NULL == iot_support_dev_list)
	{
		pr_devel("can't vmalloc %u bytes\n",MAX_DEVICE * sizeof(iot_support_dev_list_s));
		return RET_ERR;
	}
	iot_current_dev_count = 0;
	fs2416_init();
	s3c2410_init();
	return 0;
err:
	kfree(iot_cdev);
	return ret;
err_region:
	unregister_chrdev_region(dev,iot_dev_info_count);
	return ret;
}
module_init(iot_character_init);
module_exit(iot_character_cleanup);
MODULE_AUTHOR("Keven,<keven@ongoingcre.com>");
MODULE_DESCRIPTION("IOT General Driver");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");
