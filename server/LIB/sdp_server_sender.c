/**********************************************************************
*
* FILE NAME     : sdp_server_sender.c
*
* DESCRIPTION   : To send the sdp_message string to decoder.
*
*	DATE			NAME			REASON			REFERENCE
*
*	12-JAN-10	Ahmad Haris,	Initial Creation	LLD
*				Deepshikha
*	19-JAN-10	Ahmad Haris		Error Handling		LLD
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
**********************************************************************/

/**************HEADER FILE DECALARATIONS************/
#include"./../HEADER/sdp.h"

/****************Static Function Prototypes**********************/
static void sdp_timer_handler(int signo);
static void sdp_signal_handler(int signo);

/******************Global Variables for this file*****************/

char *str_ptr;					/*for storing string address*/
timer_t tid;					/*timer id*/
int closing_socket;				/*socket to be closed by handler*/
fd_set master;					/*fd set for 'select'*/

/***********************************************************************
*
* FUNCTION NAME : sdp_server_sender
*
* DESCRIPTION   : To send the sdp_message string to receiver.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : char pointer
*
***********************************************************************/

return_type sdp_server_sender(char *sdp_msg_str)
{
	int listen_socket = ZERO,		/*for listening*/
	    connection_socket = ZERO , 	/*for accepting*/
	    index = ZERO, 				/*buffer index*/
	    err = ZERO, 				/*error number*/
	    sdp_len =ZERO,				/*for strlen of sdp message*/
	    buff_len = ZERO,			/*for buffer string length*/
	    read_len = ZERO,			/*for checking 'read'*/
		fd_max = ZERO,				/*for 'select'*/
		fd_count = ZERO;			/*index for 'select'*/
	
	fd_set write_fds;				/*fd set for 'select'*/
		
	struct itimerspec	val;
	struct sockaddr_in 	server_address, 
						client_address[MAXBUFFER];
	struct sigevent 	sigx;
	socklen_t 			len;
	char 				buffer[BUFF_SIZE];
	
	TRACE("\nTRACE: Entering the function sdp_sender\n");
	
	/*initializing timer values*/
	
	sigx.sigev_signo = SIGUSR1;
	sigx.sigev_notify = SIGEV_SIGNAL;

	signal(SIGUSR1, sdp_timer_handler);
	signal(SIGINT, sdp_signal_handler);
	

	val.it_value.tv_sec = 1;
    val.it_value.tv_nsec = 500000000;
    val.it_interval.tv_sec = 1;
    val.it_interval.tv_nsec = 500000000;
	
	memset(client_address, '\0', MAXBUFFER * sizeof(struct sockaddr_in));
	
	/* creating a TCP socket */
	listen_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (listen_socket < 0)
	{
		error_handling(ERR_MAJOR, SDP_SENDER_SOCKET_ERROR, 
				"sdp_sender: Socket creation error ");

		TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
		return FAILURE;
	}

	/* initializing the server address structure */
	memset(&server_address, 0, sizeof(server_address) );
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(MYPORT);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	/* binding the protocol address to socket */
	err = bind(listen_socket,(struct sockaddr*)&server_address,	
				(socklen_t) sizeof(server_address));

	/*Error handling*/
	if (err != SUCCESS)
	{
		error_handling(ERR_MAJOR, SDP_SENDER_BIND_ERROR, "sdp_sender: Bind error ");
		TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
		return FAILURE;
	}
	
	FD_ZERO(&master); /* initialing list to zero */
	FD_ZERO(&write_fds); /* initialing list to zero */

	/* converting passive socket to listening socket*/
	err = listen(listen_socket, BACKLOG);

	/*Error handling*/
	if (err != SUCCESS)
	{
		error_handling(ERR_MAJOR, SDP_SENDER_LISTEN_ERROR, "sdp_sender: Listen error ");
		TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
		return FAILURE;
	}
	
	FD_SET(listen_socket, &master); /*add listener to master list */
	fd_max = listen_socket; /* set fdmax to max fd available */
	
	sdp_len = (int)strlen(sdp_msg_str) + 1;
	

	/* iteratively serving the client by sending the sdp message */

	while(TRUE == TRUE)
	{
		write_fds = master;
		
		str_ptr = sdp_msg_str;
		printf("\nWaiting for a ready Socket Descriptor\n");
		
		err = select(fd_max + 1, &write_fds, NULL, NULL, NULL);
		/*Error handling*/
		if (err == FAILURE)
		{
			error_handling(ERR_MAJOR, SDP_SENDER_SELECT_ERROR, 
							"sdp_sender: Select error ");
			TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
			return FAILURE;
		}
		
		printf("\nGot One.\n");

		/* run through the existing connections */
		for(fd_count = 0; fd_count <= fd_max; fd_count++)
		{
			if (FD_ISSET(fd_count, &write_fds)) /* we got one!! */
			{
				if (fd_count == listen_socket) /*handle new connections*/
				{
					len = (socklen_t)sizeof(client_address[fd_count]);
					index = connection_socket;
					connection_socket = accept(listen_socket, 
						    	    (struct sockaddr*)&client_address[connection_socket], &len);
					printf("\nSERVER : Got a new client.\n");
					/*Error handling*/
					if (connection_socket == FAILURE)
					{
						error_handling(ERR_MAJOR, 
									SDP_SENDER_ACCEPT_ERROR, 
									"sdp_sender: Accept error ");
						TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
						if(close(listen_socket) != SUCCESS)
						{
							error_handling(ERR_MAJOR, SDP_CLOSE_ERROR,
										"sdp_sender: close error.");
							TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
							return FAILURE;
						}
						return FAILURE;
					}
					/*create a timer for incoming client*/
					if(FAILURE == timer_create(CLOCK_REALTIME, &sigx, &tid))
					{
						error_handling(ERR_MAJOR, SDP_TIMER_ERROR,
							"sdp_sender: timer_create error ");
						TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
						return FAILURE;
					}
					
					err = sdp_client_check(&client_address[index]);
					if (err == SUCCESS)
					{
						printf("Registered Client Accepted.\n");
						FD_SET(connection_socket, &master);
					
						/* updating fd_max */
						if(fd_max < connection_socket) 
							fd_max = connection_socket;

						buffer[0] = '\0';
						(void)snprintf(buffer, (size_t)sizeof(int),
										"%d", sdp_len);
						buff_len = (int)strlen(buffer) + 1;
						if(FAILURE == write(connection_socket,
									buffer, (size_t) buff_len))
						{
							error_handling(ERR_MAJOR, SDP_WRITE_ERROR,
										"sdp_sender: write error.");
							TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
							return FAILURE;
						}
						
						/*********Waiting for Client message*********/
						while((read_len = (int)read(connection_socket, 
								buffer, (size_t) BUFF_SIZE)) > ZERO)
						{
							buffer[read_len - 1] = '\0';
							if( strncmp(buffer,"SDP REQUEST", 
								strlen("SDP REQUEST")) == SUCCESS)
							{
								printf("\nCLIENT : SDP REQUEST\n");
								if(FAILURE == write(connection_socket, 
											sdp_msg_str, (size_t)sdp_len))
								{
									error_handling(ERR_MAJOR, SDP_WRITE_ERROR,
										"sdp_sender: write error.");
									TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
									return FAILURE;
								}
				
								/*setting the timer*/			
								timer_settime(tid, 0, &val, NULL);
								
							/*assigning values to be used by timer_handler*/
								str_ptr = sdp_msg_str;
								closing_socket = connection_socket;
								
								printf("\nSent the Offer to client.\n");
								read_len = (int)read(connection_socket,
											buffer, (size_t)BUFF_SIZE);
								
								/*deleting the timer*/
								if(FAILURE == timer_delete(tid))
								{
									error_handling(ERR_MAJOR, SDP_TIMER_ERROR,
									"sdp_signal_handler: timer_delete error ");
									TRACE("\nTRACE: Exiting the function sdp_server_sender with failure\n");
									return FAILURE;
								}
								
								buffer[read_len - 1] = '\0';
								if( strncmp(buffer,"SDP OFFER RECEIVED", 
									strlen("SDP OFFER RECEIVED")) == SUCCESS)
								{
									printf("\nCLIENT : SDP OFFER RECEIVED\n");
									
									printf("\nSERVER: Waiting for the answer...\n");
									if(NULL == strncpy(buffer,"OK",
												strlen("OK") + 1))
									{
										error_handling(ERR_MAJOR,
													SDP_STRNCPY_ERROR,
												"sdp_sender: strncpy error.");
										TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
										return FAILURE;
									}
									buff_len = (int)strlen(buffer) + 1;
									
									/**sending ack**/
									if(FAILURE == write(connection_socket, 
											buffer, (size_t) buff_len))
									{
										error_handling(ERR_MAJOR,
														SDP_WRITE_ERROR,
												"sdp_sender: write error.");
										TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
										return FAILURE;
									}
									break;
								}
							}
						} /*end of while loop */
					}
					else
					{
						printf("\nNon Registered client rejected.\n");
						
						/*closing the socket*/
						if( FAILURE == close(connection_socket))
						{
							error_handling(ERR_MAJOR, SDP_CLOSE_ERROR,
									"sdp_sender: close error.");
							TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
							return FAILURE;
						}
					}
				}/* end of if fd_count == listener */
				
				else /*Read Answer from the ready fd*/
				{
					read_len = read(fd_count, buffer, BUFF_SIZE);
					buffer[read_len - 2] = '\0'; /*to get rid of \r\n*/
				
					if(strlen(buffer) > SIZE)
					{
						printf("Server received answer from Client %s : \n %s\n", inet_ntoa(client_address[index].sin_addr), buffer);
					
						/*******Sending Ack to client******/
						if(FAILURE == write(fd_count, 
							"SDP ANSWER RECEIVED", 19))
						{
							error_handling(ERR_MAJOR, 
									SDP_WRITE_ERROR, 
									"sdp_sender: Write error ");
							TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
							return FAILURE;
						}
					
						/***Adding client to database***/
						err = sdp_add_client(&client_address[index]);
						if (err != SUCCESS)
						{
							error_handling(ERR_MAJOR, 
									SDP_ADD_CLIENT_ERROR, 
									"sdp_sender: Add client error ");
							TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
							if(FAILURE  == close(fd_count) ||
								FAILURE == close(listen_socket))
							{
								error_handling(ERR_MAJOR, SDP_CLOSE_ERROR,
											"sdp_sender: Close error");
								return FAILURE;
							}
							return FAILURE;
						}
						printf("\nAnswering Client added to database for future reference\n");
					}
					else	 /*Answer not recieved*/
						printf("\nClient not Compatible to the sent media.\n");

					memset(buffer, 0, BUFF_SIZE);
					
					/**Closing socket**/
					if(FAILURE  == close(fd_count))
					{
						error_handling(ERR_MAJOR, SDP_CLOSE_ERROR,
									"sdp_sender: Close error");
						TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
						return FAILURE;
					}
					FD_CLR(fd_count, &master); /*updating master set*/
					
				} // end of if 2
			} // end of if 1
		}//end of for loop
				
	}//end of while loop
	
	/*closing listen_socket*/
	if(FAILURE  == close(listen_socket))
	{
		error_handling(ERR_MAJOR, SDP_CLOSE_ERROR,
					"sdp_sender: Close error");
		TRACE("\nTRACE: Exiting the function sdp_sender with failure\n");
		return FAILURE;
	}
	TRACE("\nTRACE: Exiting the function sdp_sender with success\n");
	return SUCCESS;
}//end of function

/***********************************************************************
*
* FUNCTION NAME : sdp_timer_handler
*
* DESCRIPTION   : To resend the offer if acknowledgement is not received.
*
* RETURN TYPE   : nill
*
* PARAMETERS    : integer
*
***********************************************************************/

void sdp_timer_handler(int signo /*signal number*/
					  )
{
	static int count = ZERO;
	TRACE("\nTRACE: Entering the function sdp_timer_handler.\n");
	if(count < COUNT_SIZE)
	{
		printf("\n Resending the Offer.\n");
		write(closing_socket, str_ptr, strlen(str_ptr));
		count++;
		TRACE("\nTRACE: Exiting the function sdp_timer_handler with success.\n");
	}
	else
	{
		printf("\nNo acknowledgement received from client, closing the socket.\n");
		if(FAILURE == close(closing_socket))
		{
			error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
				"sdp_timer_handler: close error ");
			TRACE("\nTRACE: Exiting the function sdp_timer_handler with failure\n");
		}
		FD_CLR(closing_socket, &master);
		TRACE("\nTRACE: Exiting the function sdp_timer_handler with success.\n");
	}
}

/***********************************************************************
*
* FUNCTION NAME : sdp_signal_handler
*
* DESCRIPTION   : To handle the inturrupt given by the user.
*
* RETURN TYPE   : nill
*
* PARAMETERS    : integer
*
***********************************************************************/

void sdp_signal_handler(int signo /*signal number*/
						)
{
	TRACE("\nTRACE: Entering the function sdp_signal_handler.\n");
	free(str_ptr);
	str_ptr = NULL;
	
	TRACE("\nTRACE: Exiting the function sdp_signal_handler with success.\n");
	exit(EXIT_FAILURE);
}
