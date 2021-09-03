#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
void main(){
	struct login{
		char id[10];
		char pass[10];
		int acc_type;
		int valid;
	};
	struct train{
		int tid;
		int totseat;
		char tname[10];
		int avseats;
	};
	struct bookings{
		int bid;
		char uid[10];
		int tid;
		int bseats;
	};
	remove("./login.txt");
	remove("./train.txt");
	remove("./bookings.txt");
	int fd1=open("./login.txt",O_CREAT|O_RDWR,0744);
	int fd2=open("./train.txt",O_CREAT|O_RDWR,0744);
	int fd3=open("./bookings.txt",O_CREAT|O_RDWR,0744);
	
/*	struct login l,l1;
	struct train t,t1;
	struct bookings b,b1;
	
	l.id = "abc";
	l.pass = "xyz";
	l.acc_type=1;
	write(fd1,&l,sizeof(l));
	lseek(fd1,sizeof(l),SEEK_SET);	
	l.id = "def";
	l.pass = "xyz";
	l.acc_type=2;
	write(fd1,&l,sizeof(l));
	//close(fd1);
	
	t.tid=123;
	t.tname = "XYZ";
	t.totseat=5;
	t.avseats=5;
	write(fd2,&t,sizeof(t));
	//close(fd2);

	b.bid=1;
	b.uid="abc";
	b.tid=123;	
	b.bseats=1;
	write(fd3,&b,sizeof(b));
	
	lseek(fd1,0,SEEK_SET);
	lseek(fd2,0,SEEK_SET);
	lseek(fd3,0,SEEK_SET);

	read(fd1,&l1,sizeof(l1));
	read(fd2,&t1,sizeof(t1));
	read(fd3,&b1,sizeof(b1));
*/
	
	
	
}

