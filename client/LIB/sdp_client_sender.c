/**********************************************************************
*
* FILE NAME     : sdp_sender.c
*
* DESCRIPTION   : To send the sdp_message string to decoder.
*
* Date        Name        Reference        Purpose
*
* 10/01/10    Navjodh     LLD             Initial creation
* 18/01/10    Neeraj      LLD             Tracing and Error Handling
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
*
**********************************************************************/

/**************HEADER FILE DECALARATIONS************/
#include"./../HEADER/sdp.h"

/***********************************************************************
*
* FUNCTION NAME : sdp_client_sender
*
* DESCRIPTION   : To send the sdp_message string to receiver.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : char *sdp_msg_str, *p_session_sockdes
*
***********************************************************************/

return_type sdp_client_sender(
                                char *sdp_msg_str,  /* pointer to SDP
                                                     Answer string */ 
                                int *p_sess_sockdes /* pointer to
                                                       socket
                                                       descriptor*/
                             )
{
    int write_ret = ZERO;      /* stores return value of write*/
    int sdp_mesg_len = ZERO;   /* stores length of SDP string*/
    int read_size = ZERO;      /* stores size of incoming SDP message*/
    char buffer[MAXBUFFER]; /*Buffer to store SDP message*/
    
    TRACE("\nTRACE: Entering function sdp_client_sender\n");
    memset(buffer, 0, MAXBUFFER);

     /*Get size of sdp string*/
    sdp_mesg_len = (int)strlen(sdp_msg_str) + 1; 

    printf("Sending SDP Answer...\n");

     /*Send sdp message using given sockdes*/
    write_ret = (int) write(
                                *p_sess_sockdes, 
                                sdp_msg_str, 
                                (size_t) sdp_mesg_len+1
                            );
    if(-1 == write_ret)
    {
        error_handling(ERR_MAJOR, SDP_WRITE_ERROR,
                       "sdp_client_sender: Write error");

                        TRACE("\nTRACE: Exiting function sdp_client_sender with failure\n");
    
	return FAILURE;
    }
    
    /*Recieve string "sdp answer recieved"*/
    read_size = (int) read(
                            *p_sess_sockdes, 
                            buffer, 
                            (size_t) MAXBUFFER
                          );
    if(-1 == read_size)
    {
        error_handling(ERR_MAJOR, SDP_READ_ERROR,
                       "sdp_client_sender: Read error");

                        TRACE("\nTRACE: Exiting function sdp_client_sender with failure\n");
    return FAILURE;
    }


   if(strcmp(buffer, "SDP ANSWER RECIEVED") == 0)
   {
       printf("SUCCESS!\n");
   }
    /*Close sockfd*/
   (void)close(*p_sess_sockdes);

   TRACE("\nTRACE: Exiting function sdp_client_sender\n");

   return SUCCESS;

}
