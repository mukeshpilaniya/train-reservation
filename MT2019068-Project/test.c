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
	}l;
	struct train{
		int tid;
		int totseat;
//		char *tname;
		int avseats;
	}t;
	struct bookings{
		int bid;
		char uid[10];
		int tid;
		int bseats;
	}b;
	int fd1=open("login.txt",O_CREAT|O_RDWR,0744);
	int fd2=open("train.txt",O_CREAT|O_RDWR,0744);
	int fd3=open("bookings.txt",O_CREAT|O_RDWR,0744);
	//printf("%d",fd1);	
	lseek(fd1,0L,SEEK_SET);
	printf("Login Credentials\n");
	while(read(fd1,&l,sizeof(l))!=0){
		//read(fd1,&l,sizeof(l));
		printf("%s\t",l.id);
		printf("%s\t",l.pass);
		printf("%d\t",l.acc_type);
		printf("%d\t",l.valid);
		printf("\n");
	}
	printf("\n-------------------------\n");
	lseek(fd2,0L,SEEK_SET);	
	printf("Train Details\n");
	while(read(fd2,&t,sizeof(t))!=0){
		//read(fd1,&l,sizeof(l));
		printf("%d\t",t.tid);
		printf("%d\t",t.totseat);
		printf("%d\t",t.avseats);
		printf("\n");
	}
	printf("\n-------------------------\n");
	lseek(fd3,0L,SEEK_SET);
	printf("Booking Details\n");
	while(read(fd3,&b,sizeof(b))!=0){
		//read(fd1,&l,sizeof(l));
		printf("%d\t",b.bid);
		printf("%s\t",b.uid);
		printf("%d\t",b.tid);
		printf("%d\t",b.bseats);
		printf("\n");
	}

}
	

