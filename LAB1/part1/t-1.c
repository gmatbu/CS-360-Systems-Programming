#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define SECTOR_SIZE 512
#define PARTITION_TABLE_OFFSET 0x1BE

int fd = 0, nr = 0;
char buf[SECTOR_SIZE];
int sector;

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

struct partition {
	u8 drive;             /* drive number FD=0, HD=0x80, etc. */

	u8  head;             /* starting head */
	u8  sector;           /* starting sector */
	u8  cylinder;         /* starting cylinder */

	u8  sys_type;         /* partition type: NTFS, LINUX, etc. */

	u8  end_head;         /* end head */
	u8  end_sector;       /* end sector */
	u8  end_cylinder;     /* end cylinder */

	u32 start_sector;     /* starting sector counting from 0 */
	u32 nr_sectors;       /* number of of sectors in partition */
};


void print_fdisk(struct partition *p)
{
	
	printf("%5u          ", p->start_sector);
	printf("%5u  ", p->nr_sectors);	
	printf("%6u     ", p->sys_type);
	printf("\n");
}

void print_extend(struct partition *p, u32 base_sector)
{
	u32 abs_start_sector;
	
	if(p->start_sector == 0)//end of link list 
	{	
		return;
	}
	if(base_sector == p->start_sector)
	{
		abs_start_sector = p->start_sector;//the first extended part 1440
	}
	else
	{
		abs_start_sector = base_sector + p->start_sector;//rest of the part
	}
	lseek(fd, (long) (abs_start_sector*512), 0);//read nth byte and write it on fd
	read(fd, buf, 512);//read fd into the buffer pointed to by buf
	p = (struct partition *)&buf[PARTITION_TABLE_OFFSET];
	printf("start_sector %u\n", abs_start_sector + p->start_sector);
	printf("nr_sectors %u\n\n", p->nr_sectors);
	print_extend(++p, base_sector);				
}


int main()
{
   struct partition *p;
  
   fd = open("vdisk", O_RDONLY);
   read(fd, buf,512);
   p = (struct partition *)&buf[PARTITION_TABLE_OFFSET];
   printf("\n------------ Linux fdisk Form -----------\n");
   printf("start_sector  nr_sectors  type\n");
   for (int i = 0; i < 4; i++)
   {
	print_fdisk(p);
		
	if (p->sys_type == 5)
	{
		printf("\n****** Extend Partition ******\n");
		print_extend(p, p->start_sector);
	}
	p++;
   }

   return 0;
}
