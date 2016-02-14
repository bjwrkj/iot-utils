#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define COLOR_START "\033[40;31m" 
#define COLOR_END "\033[0m"
#include<unistd.h>
#include <getopt.h>
#include "types.h"
#include "iot.h"
static void help(int status, const char *argv0);
#define version(v) printf("%s\n", v)
static int fd = 0;
int send_message(iot_dev_info_s *send_devinfo,char flags)
{
	int i;
	if (NULL == send_devinfo){
		fprintf(stderr, "Error: message is NULL.\n");
		return -1;
	}
	if(flags == 0){
		if(-1 == read(fd,(char *)(send_devinfo),sizeof(iot_dev_info_s))) {
			close(fd);
			for ( i = 0; i < send_devinfo->argc; i++){
				free(send_devinfo->argv[i]);
			}
			free(send_devinfo->argv[i]);
			fprintf(stderr, "Error: %s.\n",strerror(errno));
			return BOOL_FALSE;	
		} else {
				print_message(send_devinfo,0);
		}
	}
	else if(flags == 1){
		if(-1 == write(fd,(char *)(send_devinfo),sizeof(iot_dev_info_s))){
		close(fd);
			for ( i = 0; i < send_devinfo->argc; i++){
				free(send_devinfo->argv[i]);
			}
			free(send_devinfo->argv[i]);
			return BOOL_FALSE;
		}
	}
	close(fd);	
	return 0;	
}
int encapsulation_message(int type,iot_dev_info_s *send_devinfo,char *vendor_id,char *dev_name)
{
	int retval = 0;
	send_devinfo->key = type;
	if (vendor_id != NULL)
	strncpy(send_devinfo->vendor_id,vendor_id,sizeof(send_devinfo->vendor_id));
	if (dev_name != NULL)
	strncpy(send_devinfo->dev_name,dev_name,sizeof(send_devinfo->dev_name));
	return retval;
}
void print_scan_device(iot_dev_info_s *send_devinfo)
{
	
	int i;
	printf("ID\tVendor Name\tDevice Name\t\t\t  Serial Number\t\t\tStatus\t\n");
	printf("--\t-----------\t-----------\t\t\t  -------------\t\t\t------\t\n");
	for (i = 0; i < send_devinfo->current_online_devinfo[0].count; i++)
		printf("%d\t%s\t%s\t\t\t%s\t%6s\t\n",i,send_devinfo->current_online_devinfo[i].vendor_id,
										send_devinfo->current_online_devinfo[i].dev_name,
										send_devinfo->current_online_devinfo[i].serial_number,
										(send_devinfo->current_online_devinfo[i].status) ? "online":"offline");
}
void print_message(iot_dev_info_s *send_devinfo,int count)
{
	int i;
	for (i = 0; i <= count; i++){
	//	printf("key=%d\n",send_devinfo->key);
	}
	
}
/* -------------------------------------------------- */
int main(signed int argc,signed char *argv[])
{

	/* Command line options */
	bool_t scan = BOOL_FALSE;
	bool_t list = BOOL_FALSE;
	iot_dev_info_s send_devinfo;
	memset(&send_devinfo,0,sizeof(iot_dev_info_s));

	int ret = 0;
	static const char *shortopts = "hvslo:c:t:r:";
	static const struct option longopts[] = {
		{"help",	0, NULL, 'h'},
		{"version", 0, NULL, 'v'},
		{"scan",	0, NULL, 's'},
		{"list",	0, NULL, 'l'},
		{"open", 1, NULL, 'o'},
		{"close", 1, NULL, 'c'},
		{"test",	1, NULL, 't'},
		{"reset",	1, NULL, 'r'},
		{NULL,		0, NULL, 0}
	};	
	/* Parse command line options */
			int opt,type;
			opt = getopt_long(argc, argv, shortopts, longopts, NULL);
			switch (opt){
				case 's':
					scan = BOOL_TRUE;
					type = SCAN;
					break;
				case 'l':
					list = BOOL_TRUE;
					type = LIST;
					break;
				case 'o':
					type = OPEN;
					break;
				case 'c':
					type = CLOSE;
					break;
				case 't':
					type = TEST;
					break;
				case 'r':
					type = RESET;
					break;
				case 'h':
					help(0, argv[0]);
					exit(0);
					break;
				case 'v':
					version(VERSION);
					exit(0);
					break;
				default:
					help(-1,argv[0]);
					goto end;
					break;
			}
	encapsulation_message(type,&send_devinfo,optarg,argv[optind++]);
	/*open device*/	
	if (-1 == (fd = open(DEV_NAME,O_RDWR))){
		printf("%s\n",strerror(errno));
		exit(0);
	}
	/*scan Vendor && Device*/
	if (scan){
		printf("Scaning...\n");
		send_devinfo.key = type;
		send_message(&send_devinfo,0);
		print_scan_device(&send_devinfo);
	}
	/*Print Current Vendor && Device*/
	if (list){
		printf("Print Current All Device..\n");
		send_devinfo.key = type;
		send_message(&send_devinfo,0);
	}
	return 0;
end:
	return ret;
	
}/*--------------------------------------------------------- */
/* Print help message */
static void help(int status, const char *argv0)
{
	const char *name = NULL;

	if (!argv0)
		return;

	/* Find the basename */
	name = strrchr(argv0, '/');
	if (name)
		name++;
	else
		name = argv0;

	if (status != 0) {
		fprintf(stderr, "Try `%s -h' for more information.\n",
			name);
	} else {
		printf("Usage: %s [options] [vendor_id] [device_name] ...\n", name);
		printf("Smart L utility. Command line shell."
			"The part of the Smart L project.\n");
		printf("Options:\n");
		printf("\t-v, --version\tPrint version.\n");
		printf("\t-h, --help\tPrint this help.\n");
		printf("\t-s, --scan\tScand Current all of Vendor&&device "
			"\n\t\tof the daemon.\n");
		printf("\t-l, --list\tPrint Current connect all device.\n");
		printf("\t-o, --open\tOpen Specify Device.\n");
		printf("\t-c, --close\tClose Specify Device.\n");
		printf("\t-t, --test\tTest Specify Device.\n");
		printf("\t-r, --test\tReset Specify Device.\n");
	}
}
