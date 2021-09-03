#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<arpa/inet.h>
# define PORT 1337

char userid[10];
struct login{
	char id[10];
	char pass[10];
	int acc_type;
	int valid;
	}l;
struct train{
	int tid;
	int totseat;
	int avseats;
	}t;
struct bookings{
	int bid;
	char uid[10];
	int tid;
	int bseats;
	}b;

void sign_up(int commid){
	char uid[10];
	char pass[10];
	int acc_type;
	
	int fd = open("./login.txt",O_RDWR,0744);		
	read(commid,&uid,sizeof(uid));
	read(commid,&pass,sizeof(pass));
	read(commid,&acc_type,sizeof(acc_type));
		
	strcpy(l.id,uid);
	strcpy(l.pass,pass);
	l.acc_type = acc_type;
	if(acc_type==3)
		l.valid=1;	

	lseek(fd,0,SEEK_END);
	int ret = write(fd,&l,sizeof(struct login));
	//write(1,&ret,sizeof(ret));

	close(fd);		
}

void printuserid(int commid){ puts(userid);}
void log_in(int commid){
	char uid[10];
	char pass[10];
	int acc_type;
	int valid=0;
	
	int fd = open("./login.txt",O_RDWR,0744);
	lseek(fd,0,SEEK_SET);
	read(commid,&uid,sizeof(uid));
	read(commid,&pass,sizeof(pass));
	while(read(fd,&l,sizeof(l))!=0){
		if((strcmp(l.id,uid)==0) &&(strcmp(l.pass,pass)==0)){
			if(l.acc_type!=3){
				acc_type=l.acc_type;
				valid=1;
				strcpy(userid,uid);
				break;
			}
			else{
				if(l.valid==0){				
					valid=2;
					break;
				}
				else{
					acc_type=l.acc_type;
					valid=1;
					strcpy(userid,uid);
					l.valid=0;
					lseek(fd,-1*sizeof(struct login),SEEK_CUR);
					write(fd,&l,sizeof(l));
					break;		
				}
			}
		}	
		
	}
	
	write(commid,&valid,sizeof(valid));
	write(commid,&acc_type,sizeof(acc_type));
	close(fd);
}

void logout(int commid){
	char uid[10];
	char pass[10];
	int acc_type;
	int valid=1;
	
	int fd = open("./login.txt",O_RDWR,0744);
	lseek(fd,0,SEEK_SET);
	while(read(fd,&l,sizeof(l))!=0){
		if(strcmp(l.id,userid)==0 && l.valid ==0 && l.acc_type == 3){
			l.valid=1;			
			lseek(fd,-1*sizeof(struct login),SEEK_CUR);
			write(fd,&l,sizeof(l));
			break;
		}
	}
	close(fd);
	
}	
void add_train(int commid)
{
	int tid,seats;
	char train_name[10];
	int available_seats;
	

	int fd = open("./train.txt",O_RDWR,0744);
	read(commid,&tid,sizeof(tid));
	read(commid,&seats,sizeof(seats));

	int status=1;
	t.tid = tid;
	t.totseat = seats;
	t.avseats = seats;
	lseek(fd,0,SEEK_END);
	if(write(fd,&t,sizeof(t))==-1)
		status = 0;
	write(commid,&status,sizeof(status));
	close(fd);
}

void delete_train(int commid)
{
	int tid;
	int status=1;
	int fd = open("./train.txt",O_RDWR,0744);
	read(commid,&tid,sizeof(tid));
	lseek(fd,0L,SEEK_SET);
	while(read(fd,&t,sizeof(t))!=0)
	{
		if(t.tid == tid)
		{	t.tid = -1;
			t.totseat = -1;
			t.avseats = -1;
			lseek(fd,-1*sizeof(struct train),SEEK_CUR);
			if(write(fd,&t,sizeof(t))==-1)
				status = 0;
		}
	}
	
	write(commid,&status,sizeof(status));
	close(fd);
}
void search_train(int commid)
{
	int tid;
	int fd = open("./train.txt",O_RDWR,0744);
	read(commid,&tid,sizeof(tid));

	int status=0;
	lseek(fd,0,SEEK_SET);
	while(read(fd,&t,sizeof(t))!=0)
	{
		if(t.tid == tid){
			write(commid,&t,sizeof(t));
			status=1;
			break;
		}
			
	}
	
	write(commid,&status,sizeof(status));
	close(fd);
}
void modify_train(int commid)
{
	int tid,seats;
	int available_seats;
	int status=1;

	int fd = open("./train.txt",O_RDWR,0744);
	read(commid,&tid,sizeof(tid));
	read(commid,&seats,sizeof(seats));
	lseek(fd,0L,SEEK_SET);
	while(read(fd,&t,sizeof(t))!=0)
	{
		if(t.tid == tid)
		{
			t.totseat = seats;
			t.avseats = seats;
			lseek(fd,-1*sizeof(struct train),SEEK_CUR);
			if(write(fd,&t,sizeof(t))==-1)
				status = 0;
		}
	}
	
	write(commid,&status,sizeof(status));
	close(fd);
}

void book_tickets(int commid){
	int bid;
	int tid;
	int seats;
	int avseats;
	int status=1,max=0;
	int fd1 = open("./train.txt",O_RDWR,0744);
	int fd2 = open("./bookings.txt",O_RDWR,0744);
	
	read(commid,&tid,sizeof(tid));
	read(commid,&seats,sizeof(seats));
	while(read(fd1,&t,sizeof(t))!=0)
	{
		if(t.tid == tid)
		{
			avseats=t.avseats;
			break;
		}
	}
	if(avseats < seats)
	{
		status=0;
		write(commid,&status,sizeof(status));
		return;
	}
	struct flock lock1,lock2;
	lock1.l_type = F_WRLCK;
	lock1.l_start = 0;
	lock1.l_len = 0;
	lock1.l_whence = SEEK_SET;
	lock1.l_pid = getpid();
	
	fcntl(fd1, F_SETLKW, &lock1);

	lock2.l_type = F_WRLCK;
	lock2.l_start = 0;
	lock2.l_len = 0;
	lock2.l_whence = SEEK_SET;
	lock2.l_pid = getpid();
	
	fcntl(fd2, F_SETLKW, &lock2);
	lseek(fd1,0,SEEK_SET);
	while(read(fd1,&t,sizeof(t))!=0)
	{
		if(t.tid == tid)
		{
			t.avseats = t.avseats-seats;
			lseek(fd1,-1*sizeof(struct train),SEEK_CUR);
			if(write(fd1,&t,sizeof(t))==-1)
				status = 0;
			break;
		}
	}
	if(status==0){
		lock1.l_type=F_UNLCK;
		fcntl(fd1, F_SETLKW, &lock1);
		lock2.l_type=F_UNLCK;
		fcntl(fd2, F_SETLKW, &lock2);
		write(commid,&status,sizeof(status));
		close(fd1);
		close(fd2);
		printf("Foo was here");
		return;
	}
	lseek(fd2,0,SEEK_SET);
	//	lseek(fd2,-1*sizeof(struct bookings),SEEK_CUR);
	while(read(fd2,&b,sizeof(b))!=0)
	{
		if(max < b.bid)
		max = b.bid;
	}
	lseek(fd2,0,SEEK_END);
	b.bid = max+1;
	b.tid = tid;
	b.bseats = seats;
	strcpy(b.uid,userid);
	
	lseek(fd2,0,SEEK_END);
	write(fd2,&b,sizeof(struct bookings));
	
	lock1.l_type=F_UNLCK;
		fcntl(fd1, F_SETLKW, &lock1);	
	lock2.l_type=F_UNLCK;
		fcntl(fd2, F_SETLKW, &lock2);
	status=1;
	write(commid,&status,sizeof(status));

	close(fd1);
	close(fd2);
	
}

void view_booking(int commid){
	
	int entries=0;
	int fd1 = open("./bookings.txt",O_RDWR,0744);
	lseek(fd1,0L,SEEK_SET);
	while(read(fd1,&b,sizeof(struct bookings))!=0)
	{
		if(strcmp(b.uid,userid)==0)
			entries++;
	}
	write(commid,&entries,sizeof(entries));

	lseek(fd1,0L,SEEK_SET);
	while(read(fd1,&b,sizeof(struct bookings))!=0)
	{
		if(strcmp(b.uid,userid)==0)
			write(commid,&b,sizeof(struct bookings));
	}
	close(fd1);
	
}

void cancel(int commid){
	int bid;
	int tid;
	int seats;
	int avseats;
	int status=1;
	int fd1 = open("./train.txt",O_RDWR,0744);
	int fd2 = open("./bookings.txt",O_RDWR,0744);
	
	read(commid,&bid,sizeof(bid));
	while(read(fd2,&b,sizeof(b))!=0)
	{
		if(b.bid == bid)
		{
			tid=b.tid;
			seats=b.bseats;
			break;
		}
	}
	
	struct flock lock1,lock2;
	lock1.l_type = F_WRLCK;
	lock1.l_start = 0;
	lock1.l_len = 0;
	lock1.l_whence = SEEK_SET;
	lock1.l_pid = getpid();
	
	fcntl(fd1, F_SETLKW, &lock1);

	lock2.l_type = F_WRLCK;
	lock2.l_start = 0;
	lock2.l_len = 0;
	lock2.l_whence = SEEK_SET;
	lock2.l_pid = getpid();
	
	fcntl(fd2, F_SETLKW, &lock2);
	lseek(fd1,0,SEEK_SET);
	while(read(fd1,&t,sizeof(t))!=0)
	{
		if(t.tid == tid)
		{
			t.avseats = t.avseats+seats;
			lseek(fd1,-1*sizeof(struct train),SEEK_CUR);
			if(write(fd1,&t,sizeof(t))==-1)
				status = 0;
			break;
		}
	}
	if(status==0){
		lock1.l_type=F_UNLCK;
		fcntl(fd1, F_SETLKW, &lock1);
		lock2.l_type=F_UNLCK;
		fcntl(fd2, F_SETLKW, &lock2);
		write(commid,&status,sizeof(status));
		close(fd1);
		close(fd2);
		printf("Foo was here");
		return;
	}
	lseek(fd2,0,SEEK_SET);
	//lseek(fd2,-1*sizeof(struct bookings),SEEK_CUR);
	//read(fd2,&b,sizeof(b));
	//bid=b.bid+1;
	while(read(fd2,&b,sizeof(b))!=0)
	{
		if(b.bid == bid)
		{
			b.bid=-1;b.tid=-1;b.bseats=-1;strcpy(b.uid,"/0");
			lseek(fd2,-1*sizeof(struct bookings),SEEK_CUR);
			if(write(fd2,&b,sizeof(b))==-1)
				status = 0;
			break;
		}
	}

	
	lock1.l_type=F_UNLCK;
		fcntl(fd1, F_SETLKW, &lock1);	
	lock2.l_type=F_UNLCK;
		fcntl(fd2, F_SETLKW, &lock2);
	status=1;
	write(commid,&status,sizeof(status));

	close(fd1);
	close(fd2);
}

void update(int commid){
	int bid;
	int tid;
	int newseats;
	int seats;
	int avseats;
	int status=1;
	int fd1 = open("./train.txt",O_RDWR,0744);
	int fd2 = open("./bookings.txt",O_RDWR,0744);
	
	read(commid,&bid,sizeof(bid));
	read(commid,&newseats,sizeof(newseats));
	while(read(fd2,&b,sizeof(b))!=0)
	{
		if(b.bid == bid)
		{
			seats=b.bseats;
			tid = b.tid;
			break;
		}
	}
	while(read(fd1,&t,sizeof(t))!=0)
	{
		if(t.tid == tid)
		{
			avseats=t.avseats;
			break;
		}
	}
	if(avseats+seats < newseats)
	{
		status=0;
		write(commid,&status,sizeof(status));
		return;
	}
	struct flock lock1,lock2;
	lock1.l_type = F_WRLCK;
	lock1.l_start = 0;
	lock1.l_len = 0;
	lock1.l_whence = SEEK_SET;
	lock1.l_pid = getpid();
	
	fcntl(fd1, F_SETLKW, &lock1);

	lock2.l_type = F_WRLCK;
	lock2.l_start = 0;
	lock2.l_len = 0;
	lock2.l_whence = SEEK_SET;
	lock2.l_pid = getpid();
	
	fcntl(fd2, F_SETLKW, &lock2);
	lseek(fd1,0,SEEK_SET);
	while(read(fd1,&t,sizeof(t))!=0)
	{
		if(t.tid == tid)
		{
			t.avseats = t.avseats+seats-newseats;
			lseek(fd1,-1*sizeof(struct train),SEEK_CUR);
			if(write(fd1,&t,sizeof(t))==-1)
				status = 0;
			break;
		}
	}
	if(status==0){
		lock1.l_type=F_UNLCK;
		fcntl(fd1, F_SETLKW, &lock1);
		lock2.l_type=F_UNLCK;
		fcntl(fd2, F_SETLKW, &lock2);
		write(commid,&status,sizeof(status));
		close(fd1);
		close(fd2);
		printf("Foo was here");
		return;
	}
	lseek(fd2,0,SEEK_SET);
	
	while(read(fd2,&b,sizeof(b))!=0)
	{
		if(b.bid == bid)
		{
			b.bseats = newseats;
			lseek(fd2,-1*sizeof(struct bookings),SEEK_CUR);
			if(write(fd2,&b,sizeof(b))==-1)
				status = 0;
			break;
		}
	}

	
	lock1.l_type=F_UNLCK;
		fcntl(fd1, F_SETLKW, &lock1);	
	lock2.l_type=F_UNLCK;
		fcntl(fd2, F_SETLKW, &lock2);
	status=1;
	write(commid,&status,sizeof(status));

	close(fd1);
	close(fd2);
	
}
int func(int commid)
{
	int ch;
	int retval=1; 
	read(commid,&ch,sizeof(ch));
	if(ch>=1 && ch<=10){
		if(ch == 1)
			{sign_up(commid);retval=1;}
		if(ch == 2)
			{log_in(commid);retval=1;}
	}
	if(ch>20 && ch <31)
	{
		switch(ch)
		{	
			case 21: add_train(commid);retval=1;break;
			case 22: delete_train(commid);retval=1;break;
		 	case 23: modify_train(commid);retval=1;break;
			case 24: search_train(commid);retval=1;break;
			case 25: retval=1;break;
	    	}

	}
	if(ch>10&&ch<20)
	{
		switch(ch)
		{
			case 11:book_tickets(commid);retval=1;break;
			case 12:view_booking(commid);retval=1;break;
			case 13:cancel(commid);retval=1;break;
			case 14:update(commid);retval=1;break;
			case 15:logout(commid);retval=1;break;
		}
	}
	if(ch==50)
		{ printuserid(commid);retval=1;}	

	if(ch==100){
		retval = 0;
	}
	return retval;
}
int main()
{
	int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr));  
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
   if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli);  
    while(1){
    	connfd = accept(sockfd, (struct sockaddr*)&cli, &len); 
    	if (connfd < 0) { 
        	printf("server acccept failed...\n"); 
        	exit(0); 
	}
    	else	
        	printf("server acccept the client...\n");
	if(fork()==0){
		int ret=1;
		while(ret)
			ret=func(connfd); 
		printf("Closing Connection...");
		close(connfd);
		exit(0);
	}
	else
		close(connfd);
    } 
	close(sockfd);
    
}
