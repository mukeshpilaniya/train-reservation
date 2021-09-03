#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<arpa/inet.h>
# define PORT 1337

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

void train_booking_menu(int sockfd){
	int ch;
	do{
		system("clear");
		printf("\n----Menu for User/Agent----\n");
		printf("1.Book Ticket\n");
		printf("2.View Previous Bookings\n");
		printf("3.Cancel Booking\n");
		printf("4.Update Booking\n");
		printf("5.Go back\n");
		printf("Enter your choice: ");
		scanf("%d",&ch);
		ch=ch+10;
		write(sockfd,&ch,sizeof(ch));
		ch=ch-10;
		int tid, bid;
		int seats;
		int status;
		switch(ch){
			case 1: printf("\n Enter Train id:");
				scanf("%d",&tid);
				printf("\nEnter No. of seats:");
				scanf("%d",&seats);
				
				write(sockfd,&tid,sizeof(tid));
				write(sockfd,&seats,sizeof(seats));
				read(sockfd,&status,sizeof(status));

				if(status == 1)
					printf("\nTransaction Successful");
				else printf("\nTransaction failed : Not Enough Seats");
				break;

			case 2:	read(sockfd,&status,sizeof(status));
				if(status!=0)			
					printf("%d\n",status);
				else
					printf("\nNo Bookings");
				sleep(2);
				while(status!=0){
					read(sockfd,&b,sizeof(struct bookings));
					printf("%d\t",b.bid);
					printf("%d\t",b.tid);
					printf("%s\t",b.uid);
					printf("%d\t",b.bseats);
					printf("\n----------------\n");
					status-=1;
				}sleep(3);			
				//if(status==1)
				//	printf("\nTransaction Successful");
				//else 	printf("\nTransaction UnSuccessful");
				break; 

			case 3: printf("\nEnter booking id:");
				scanf("%d",&bid);
				write(sockfd,&bid,sizeof(bid));
				read(sockfd,&status,sizeof(status));

				if(status==1)
					printf("\nTransaction Successful");
				else 	printf("\nTransaction UnSuccessful");

				break;
			case 4: printf("Enter Booking Id:");
				scanf("%d",&bid);
				printf("\nEnter New No: of Seats:");
				scanf("%d",&seats);
				write(sockfd,&bid,sizeof(bid));
				write(sockfd,&seats,sizeof(seats));
				read(sockfd,&status,sizeof(status));

				if(status==1)
					printf("\nTransaction Successful");
				else 	printf("\nTransaction UnSuccessful");
				break;
			case 5: ch=ch+10; write(sockfd,&ch,sizeof(ch));return;
			default:printf("\nWrong Choice!!!");break;

		}

	}while(1);	
}
void admin_menu(int sockfd){
	int ch;
	do{
		system("clear");
		printf("\n----Menu for Admin----\n");
		printf("1.Add Train\n");
		printf("2.Delete Train \n");
		printf("3.Modify Train\n");
		printf("4.Search Train\n");
		printf("5.To go Back\n");
		printf("\nEnter your choice:");
		scanf("%d",&ch);
		ch=ch+20;
		write(sockfd,&ch,sizeof(ch));
		ch=ch-20;

		int tid, bid, uid;
		int seats;
		int status;
		switch(ch){
			case 1: printf("\n Enter Train id:");
				scanf("%d",&tid);
				printf("\nEnter Total No. of seats:");
				scanf("%d",&seats);
				
				write(sockfd,&tid,sizeof(tid));
				write(sockfd,&seats,sizeof(seats));
				read(sockfd,&status,sizeof(status));

				if(status == 1)
					printf("\nTransaction Successful");
				else printf("\nTransaction failed");
				break;

			case 2: printf("\nEnter Trainid:");
				scanf("%d",&tid);
				write(sockfd,&tid,sizeof(tid));
				read(sockfd,&status,sizeof(status));
				if(status==1)
					printf("\nTransaction Successful");
				else 	printf("\nTransaction UnSuccessful");
				break; 

			case 3: printf("\nEnter train id:");
				scanf("%d",&tid);
				printf("\nEnter New Total Seats:");
				scanf("%d",&seats);
				write(sockfd,&tid,sizeof(tid));
				write(sockfd,&seats,sizeof(seats));
				read(sockfd,&status,sizeof(status));

				if(status==1)
					printf("\nTransaction Successful");
				else 	printf("\nTransaction UnSuccessful");

				break;
			case 4: printf("Enter Train Id:");
				scanf("%d",&tid);
				write(sockfd,&tid,sizeof(tid));
				read(sockfd,&t,sizeof(t));
				read(sockfd,&status,sizeof(status));
				
				if(status==1){
					printf("\nTrain Found");
					printf("\nTrain No:%d",t.tid);
					printf("\nTotal Seats%d",t.totseat);
					printf("\nAvailable Seats%d",t.avseats);
					printf("\n");
					sleep(3);
				}
				else 	printf("\nTrain Not Found");
				break;
			case 5: return;
			default:printf("\nWrong Choice!!!");break;

		}

	}while(1);	
}


void main(){
	
	
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;
	sockfd = socket(AF_INET, SOCK_STREAM,0);
	if(sockfd==-1){
		printf("\nSocket Createion failed...");
		exit(1);
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr= INADDR_ANY;
	servaddr.sin_port=htons(PORT);
	if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))!=0){
		printf("FAILED TO CONNECT WITH SERVER");
		exit(1);
	}
	else printf("\nConnected to Server");
	
	//menu for train booking
	int ch;
	do{
		system("clear");
		printf("-----------------------------TRAIN RESERVATION SYSTEM-------------------------\n");
		printf("----Menu for account creation----\n");
		printf("1.Sign Up\n");
		printf("2.Log in\n");
		printf("3.Exit\n");
		printf("Enter your choice:");
		scanf("%d",&ch);
		printf("\n");
		write(sockfd,&ch,sizeof(ch));
		int acc_type;
		char name[10],pass[10];
		if(ch == 1)
		{

			printf("\nEnter Name : ");
			scanf("%s",name);
			printf("Enter Password: ");
			scanf("%s",pass);
			
			printf("\n1 for Admin \n2 for Agent \n3 for User\n");
			printf("Enter Account type: ");
			scanf("%d",&acc_type);
		
			write(sockfd,&name,sizeof(name));
			write(sockfd,&pass,sizeof(pass));
			write(sockfd,&acc_type,sizeof(acc_type));

		}
		else if(ch == 2){
			printf("\nEnter User name:");
			scanf("%s",name);
			printf("\nEnter Password: ");
			scanf("%s",pass);
		
			write(sockfd,&name,sizeof(name));
			write(sockfd,&pass,sizeof(pass));

			int valid_login,acc_type1;
			read(sockfd,&valid_login,sizeof(valid_login));
			read(sockfd,&acc_type1,sizeof(acc_type1));
			if(valid_login == 1 && acc_type1!=1)
				train_booking_menu(sockfd);
			else if(valid_login == 1 && acc_type1==1)
				admin_menu(sockfd);
			else if(valid_login == 2){
				printf("\nMultiple Login Not allowed!!!");
				sleep(3);
			}
			else{
				printf("\nInvalid credentials\n");
				sleep(3);
			}

		}
		else if(ch==4){//to initiate func to print userid
			ch=50;write(sockfd,&ch,sizeof(ch));}
		else if(ch == 3){
			ch=100;
			write(sockfd,&ch,sizeof(ch));printf("\n Exiting");sleep(1);
			exit(0);}
		else
			printf("Wrong choice\n");
		
	}while(1);

}
