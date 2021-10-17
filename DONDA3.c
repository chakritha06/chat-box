/*
Chakritha Reddy Dondapati

*/
#include<stdio.h>
#include<string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<stdlib.h>


#define max_no_of_clints 10  // max vqlue.

int jndclnts=0; // cnt of clnts


struct clntvarble  // clnt info
{
	int scktid;      //sckt decr
    char clnamee[100];  //clnt nme
};


struct clntvarble *listofClients[max_no_of_clints]; //cli inf.


void *threadmethod(void *argmnnntsss);  //thred fn.

int xyz(int inputargss)
{
	int c=0;
	if (inputargss != 2)
    {
		c++;
        printf("usage:./prog3svr <svr_port>\n");
        
    }
	return c;
}






void *threadmethod(void *argmnnntsss)  // thred method
{
	struct clntvarble *newclint = (struct clntvarble *)argmnnntsss;  //clnt inf
	
	printf("Client (%d): Connection Handler Assigned\n",newclint->scktid);
	
	for(int chkritha=1;chkritha>=1;chkritha++)//loop to make server listn 
	{
		//variables decln 
		char mesagefirst[4096];
		int cp=0;
		int pc=0;
		int mp=0;
		char srng[100] = {'\0'};
		
		char cnme[100] = {'\0'};
		char cmsg[2048] = {'\0'};
		char cli_bmsg[2048] = {'\0'};
		int ajnd=0;
		
		//qury recvng
		int rlrnht = recv(newclint->scktid, mesagefirst, 4096 ,0);
		mesagefirst[rlrnht] = '\0';
		strncpy(srng,mesagefirst,4);
		mp=strlen(mesagefirst);
		
		// JOIN LIST MESG BCST QUIT logics
		if(strncmp(srng,"JOIN",4)==0)
		{
			cp=1;
			
			if(mesagefirst[4]==' ')
			{
				int chk=5;
				while( mesagefirst[chk]!='\0'&& chk<mp-1)  
				{	
					char user[10]={'\0'};
					sprintf(user,"%c",mesagefirst[chk]); 
					strcat(cnme,user);
					chk++;
				}
				cnme[strlen(cnme)]='\0';
			}
		}
		else if(strncmp(srng,"LIST",4)==0)
		{
			cp=2;
		}
		else if(strncmp(srng,"MESG",4)==0)
		{
			cp=3;
			int mp=strlen(mesagefirst);
			int chk=0;
			if(mesagefirst[4] ==' ')
			{
				int us=5;
				while( mesagefirst[us]!=' ')
				{	
					char user[10]={'\0'};
					sprintf(user,"%c",mesagefirst[us]); 
					strcat(cnme,user);
					us++;
				}
				
				chk=us;

				if(mesagefirst[chk]==' ')
				{	
					chk++;
					while( mesagefirst[chk]!='\0'&& chk<mp-1) 
					{	
						char mesg1[10]={'\0'};
						sprintf(mesg1,"%c",mesagefirst[chk]); 
						strcat(cmsg,mesg1);
						chk++;
					}
				}
			}
		}
			
		else if (strncmp(srng,"BCST",4)==0)
		{
			int mp=strlen(mesagefirst);
			cp=4;
			if(mesagefirst[4]==' ')
			{
				int md=5;
				while( mesagefirst[md]!='\0'&& md<mp-1)  
				{	
					char sentmsgg[10]={'\0'};
					sprintf(sentmsgg,"%c",mesagefirst[md]); 
					strcat(cli_bmsg,sentmsgg);
					md++;
				}
			}
		}
		else if (strncmp(srng,"QUIT",4)==0)
		{
			cp=5;
		}
		
		
		
		switch(cp)//logic
		{
	
			case 1: //add to db.
					
					
					
					pc=strlen(newclint->clnamee);
					if(pc==0)
					{
						int n1=0;
						n1=strlen(cnme);
						jndclnts++;
						if( jndclnts > max_no_of_clints) // more 
						{
							printf("Client (%d): Database Full. Disconnecting User\n",newclint->scktid);  
							printf("Error: Too Many Clients Connected\n");
							send(newclint->scktid,"Too Many Users. Disconnecting User.\n",strlen("Too Many Users. Disconnecting User.\n"),0); //status
							close(newclint->scktid); //close
							jndclnts--;  //decrse
							newclint=NULL; //freeup mmry
							pthread_exit(NULL);
						}
						
						int mvk=0;
						int chk=0;
						
						while(mvk<max_no_of_clints)
						{		
					
								if(listofClients[mvk])
								{	
									if(strncmp(listofClients[mvk]->clnamee,cnme,strlen(listofClients[mvk]->clnamee))==0 && strncmp(listofClients[mvk]->clnamee,cnme,strlen(cnme)-1)==0)  
									{
										ajnd=1;
										
										break;
									}
								}
								
								mvk++;
						}
						
						
						
					
						
						if (n1!=0 && ajnd==0 ) //  add clnt to db.
						{	 
							int chk=0;
							strncpy(newclint->clnamee,cnme,strlen(cnme)-1);  
							
							while(chk<max_no_of_clints)
							{
								if (!listofClients[chk])  
								{
									listofClients[chk] = newclint;
									break;
								}
								chk++;
							}
							printf("Client (%d): JOIN %s\n",newclint->scktid,newclint->clnamee);  
							strncpy(mesagefirst,mesagefirst,sizeof(mesagefirst)-1);
							strcat(mesagefirst," Request Accepted\n");
							send(newclint->scktid,mesagefirst,strlen(mesagefirst),0);  //sts to cli
						}
						else  //else comdn
						{
							if(ajnd==1)
							{
								printf("Client (%d): User Already Registered. Discarding JOIN\n",newclint->scktid);
								char hr[50];
								char simsg[1000]={'\0'};
								int chk=0;
								while(chk<max_no_of_clints)  
								{
									if(listofClients[chk])
									{
										if (strncmp(listofClients[chk]->clnamee,cnme,strlen(listofClients[chk]->clnamee))==0)  
										{
											sprintf(hr,"%d", listofClients[chk]->scktid);
											break;
										}
									}
									chk++;
							}
							strcpy(simsg,"User Already Registered : Username - ");
							strncat(simsg,cnme,strlen(cnme)-1);
							strcat(simsg," FD - ");
							strcat(simsg,hr);
							strcat(simsg,"\n");
							send(newclint->scktid,simsg,strlen(simsg),0); //sts to serv.
							memset(simsg,0,1000);
							}
						
							else
							{
								
								printf("Client (%d): Unrecognizable Message. Discarding UNKNOWN Message\n",newclint->scktid);  //prnt on ser 
								pc=strlen(newclint->clnamee);
								if(pc!=0)   
									send(newclint->scktid,"Unknown Message. Discarding UNKNOWN Message.\n",strlen("Unknown Message. Discarding UNKNOWN Message.\n") ,0); //sending status message to client.
								else 
									send(newclint->scktid,"Unregistered User. Use \"JOIN <username>\" to Register.\n",strlen("Unregistered User. Use \"JOIN <username>\" to Register.\n"), 0);  //sending status message to client.
							}
						}
					}
					else //  clnt.
					{	
						printf("Client (%d): User Already Registered. Discarding JOIN\n",newclint->scktid);
						char hr[50];
						char simsg[1000]={'\0'};
						int chk=0;
						while(chk<max_no_of_clints)  
						{
							if(listofClients[chk])
							{
								if (strncmp(listofClients[chk]->clnamee,cnme,strlen(listofClients[chk]->clnamee))==0)  
								{
									sprintf(hr,"%d", listofClients[chk]->scktid);
									break;
								}
							}
							chk++;
						}
						strcpy(simsg,"User Already Registered : Username - ");
						strcat(simsg,newclint->clnamee);
						strcat(simsg,"\t,FD - ");
						strcat(simsg,hr);
						strcat(simsg,"\n");
						send(newclint->scktid,simsg,strlen(simsg),0); //stsus to sever.
						memset(simsg,0,1000);
					}
					break;
		

			case 2:  // LIST .
					printf("Client (%d): LIST\n",newclint->scktid);
					pc=strlen(newclint->clnamee);
					if(pc==0) 
					{
						printf("Unable to Locate Client (%d) in Database. Discarding LIST\n",newclint->scktid);    
						send(newclint->scktid,"Unregistered User. Use \"JOIN <username>\" to Register.\n",strlen("Unregistered User. Use \"JOIN <username>\" to Register.\n"), 0); //sending status message to server
					}
					//db to clnt
					else							
					{
						char simsg[1000]="USERNAME\tFD\n----------------------------\n";
						//rcrd tfind from db
						int chk=0;
						while(chk<max_no_of_clints)  
						{
							if(listofClients[chk])
							{
								char hr[50];
								strcat(simsg,listofClients[chk]->clnamee);
								strcat(simsg,"		");
								sprintf(hr,"%d",listofClients[chk]->scktid);
								strcat(simsg,hr);
								strcat(simsg,"\n");
							}
							chk++;
						}	
						strcat(simsg,"----------------------------\n");
						send(newclint->scktid,simsg,strlen(simsg),0);//stat to clnt  
						memset(simsg,0,1000);
					}
					break;
					
			case 3: //MESG 
					pc=strlen(newclint->clnamee);
				
					if(pc==0)
					{
						printf("Unable to Locate Client (%d) in Database. Discarding MESG\n",newclint->scktid);    // srvr msg
						
						send(newclint->scktid,"Unregistered User. Use \"JOIN <username>\" to Register.\n",strlen("Unregistered User. Use \"JOIN <username>\" to Register.\n"), 0); //sts to clin
						
					}
				
					else
					{	
						int rskt=0;
						int rpntnt=0;;    
						int chk=0;
						int md=0;
						int us=0;
						while(chk<max_no_of_clints)  
						{
							if(listofClients[chk])
							{
							//valdn
								if(strncmp(listofClients[chk]->clnamee,cnme,strlen(cnme)) == 0 && strncmp(listofClients[chk]->clnamee,cnme,strlen(listofClients[chk]->clnamee)) == 0) 
								{
									rskt = listofClients[chk]->scktid;
									rpntnt=1;  
									break;
								}
							}
							chk++;
						}
					
						us=strlen(cmsg);
						if(strcmp(newclint->clnamee,cnme)!=0) // valdn to self
						{
							if(us!=0)
							{	
								if(rpntnt==1) 
								{	
									char simsg[4096]={'\0'};
									char sentmsgg[2048]={'\0'};
									char snmeee[100]={'\0'};
									char sndr[100]={'\0'};
									strncat(sentmsgg,cmsg,strlen(cmsg)-1);
									strncpy(snmeee,newclint->clnamee,strlen(newclint->clnamee)+1);
									strcat(simsg,"FROM ");
									sprintf(sndr,"%s :",snmeee);
									strcat(simsg,sndr);
									strcat(simsg,sentmsgg);
									strcat(simsg,"\n");
									simsg[strlen(simsg)]='\0';
									send(rskt,simsg,strlen(simsg),0);//stats to clnt
									memset(simsg,0,2048);
								}
								
								else
								{ 	
									//if the recipient is not registered in databse.
									printf("Unable to Locate Recipient (%s) in Database. Discarding MESG.\n",cnme); 
									char sentmsgg[1000]="Unknown Recipient (";
									strcat(sentmsgg,cnme);
									strcat(sentmsgg,"). MESG Discarded.\n");
									send(newclint->scktid,sentmsgg,strlen(sentmsgg),0);  //stats to cli
									memset(sentmsgg,0,1000);
								}	
							}
							else
							{
								//msg null
								printf("Message is NULL  Discarding MESG.\n"); 
								char sentmsgg[1000]="Message is NULL-";
								strcat(sentmsgg," MESG Discarded.\n");
								send(newclint->scktid,sentmsgg,strlen(sentmsgg),0); 
								memset(sentmsgg,0,1000);
							}
						}
							
						else 
						{	
							//valdn itself
							printf("client (%d) :Unable to send message sndr and receiver are same.\n",newclint->scktid); 
							char sentmsgg[1000]="Client can't send message to self -";
							strcat(sentmsgg," MESG Discarded.\n");
							send(newclint->scktid,sentmsgg,strlen(sentmsgg),0); 
							memset(sentmsgg,0,1000);
						}
							
					
							
					}
					
						
					
					break;
						
						
			case 4:  
					//  BCST 
					
					pc=strlen(newclint->clnamee);
					if(pc==0) // valdn clint
					{	
						//true case
						printf("Unable to Locate Client (%d) in Database. Discarding BCST\n",newclint->scktid);    
						send(newclint->scktid,"Unregistered User. Use \"JOIN <username>\" to Register.\n",strlen("Unregistered User. Use \"JOIN <username>\" to Register.\n"), 0); //stats to clntt.
					}
					else 
					{
						//logic sending msg
						int chk=0;
						if(strlen(cli_bmsg)==0)
						{
								//null msg
								printf("Message is NULL  Discarding BCST.\n"); 
								char sentmsgg[1000]="Message is NULL(";
								strcat(sentmsgg,"). BCST Discarded.\n");
								send(newclint->scktid,sentmsgg,strlen(sentmsgg),0); 
								memset(sentmsgg,0,1000);
								
								
						}
						else
						{
							
							while(chk<max_no_of_clints)  
					
							{
								if(listofClients[chk]) //sent to clnts
								{
									char simsg[1000]={'\0'};
									strcat(simsg,"FROM ");
									strncat(simsg,newclint->clnamee,strlen(newclint->clnamee)+1);
									strcat(simsg,": ");
									strcat(simsg,cli_bmsg);
									strcat(simsg,"\n");
									if(listofClients[chk]->scktid != newclint->scktid)
										send(listofClients[chk]->scktid,simsg,strlen(simsg),0); 
									memset(simsg,0,1000);
								}
							
								chk++;
							}
						}
					
					}
					
					break;
				
			case 5:
					//QUIT 
					printf("Client (%d): QUIT\n",newclint->scktid);
					pc=strlen(newclint->clnamee);
					//not rgstrd
					if(pc==0)		
					{	
			
						printf("Unable to Locate Client (%d) in Database. Disconnecting User.\n",newclint->scktid);
						printf("Client (%d): Disconnecting User.\n",newclint->scktid);
						close(newclint->scktid); //closing the client socket connection.
						newclint=NULL;// freeing up the struct variable memory.
						pthread_exit(NULL);//closing  the thread upon quit.
					}
					else
					{
					
						printf("Client (%d): Disconnecting User.\n",newclint->scktid);
						//closng skt
						int md=0;
						while(md<max_no_of_clints)   //removing clnt fro db
						{
							if(listofClients[md])
							{
								if (strcmp(listofClients[md]->clnamee,newclint->clnamee)==0)
								{	
							
									
									listofClients[md]=NULL;
									break;
								}
							}
							md++;
						}
						close(newclint->scktid);//clse clnt skt
						newclint=NULL;//free up
						jndclnts--; //decrmnt
						pthread_exit(NULL);
						
					}
					break;
					
					
			default: 
					
					printf("Client (%d): Unrecognizable Message. Discarding UNKNOWN Message\n",newclint->scktid);  
					pc=strlen(newclint->clnamee);
					if(pc==0)  
						send(newclint->scktid,"Unregistered User. Use \"JOIN <username>\" to Register.\n",strlen("Unregistered User. Use \"JOIN <username>\" to Register.\n"), 0); 			
							
					else 
						send(newclint->scktid,"Unknown Message. Discarding UNKNOWN Message.\n",strlen("Unknown Message. Discarding UNKNOWN Message.\n") ,0);			
					
		
		}	
	
	}

}      

int main ( int ainpt, char **argums)
{
	int sfd;     //skt decrptor
	int sktcli;   // o/p cli
	int pidd;      //portno
	int chk=0;          //loops
	int bndopt;      //o/p bind
	int lstnop;    //o/p lisn
	struct sockaddr_in serveradress; //serv addrs
	struct sockaddr_in clntadress; //clnt addrs
	int lencln;           //clnt adrs lenth
	pthread_t tdclc[50];   // thread id
	
	/*vldn*/
	
	int abc= xyz(ainpt);
	if(abc !=0)
	{
		exit(1);
	}
	
	/*skt fle descrptor */
	sfd=socket(AF_INET,SOCK_STREAM,0);
	
	/*vldng skt */	
	if(sfd==-1)
	{
		printf("\n : Connection Failed could not create socket");
		exit(1);
	}
	
	pidd = atoi(argums[1]);
	
	//conf address
  
	bzero((char*)&serveradress, sizeof(serveradress));
    serveradress.sin_family = AF_INET;
    serveradress.sin_addr.s_addr = htonl(INADDR_ANY); //set prntno
    serveradress.sin_port=htons(pidd); //set ip
	
	lencln=sizeof(clntadress);
    /*port to ip*/
	bndopt= bind(sfd,(struct sockaddr *)&serveradress, sizeof(serveradress));
	
	if(bndopt == -1)
	{
		printf("\n Error:Connection Failed could not bind address with socket \n ");
		exit(1);		
	}
	
	lencln=sizeof(clntadress);
	//lstn fun
	lstnop=listen(sfd,10);
	
	if(lstnop<0)   
	{
		printf("Error on listen\n");
		exit(1);
	}
	/*wait for clnt*/
	printf("Waiting for Incoming Connections...\n");
	
	
	//Accept
	while( (sktcli = accept(sfd, (struct sockaddr *)&clntadress, &lencln)) )
	{
		
		struct clntvarble *clent = (struct clntvarble *)malloc(sizeof(struct clntvarble));
		
		clent -> scktid = sktcli;
		
		printf("Client (%d): Connection Accepted\n",clent -> scktid);
		
		// client creats  thread 
		if(pthread_create(&tdclc[chk++], NULL ,&threadmethod, (void *)clent ) != 0)
		{
			printf("could not create thread\n");
			exit(1);
		}	
		
		
	}
	//close thrd
	while(chk!=0)  
	{
		pthread_join(tdclc[chk],NULL);
		chk--;
		
	}

	return 0;
}







