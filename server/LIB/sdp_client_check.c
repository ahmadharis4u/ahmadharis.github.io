/**********************************************************************
*
* FILE NAME     : sdp_client_check.c
*
* DESCRIPTION   : To add the media field to session structure.
*
*	DATE			NAME			REASON			REFERENCE
*
*	14-JAN-10	Ahmad Haris		Initial Creation	LLD
*	19-JAN-10	Ahmad Haris		Error Handling		LLD
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
**********************************************************************/

/*************HEADER FILES INCLUSION*************/
#include"./../HEADER/sdp.h"


/***********************************************************************
*
* FUNCTION NAME : sdp_client_check
*
* DESCRIPTION   : To add the media field to session structure.
*
* RETURN TYPE   : return_type
*
***********************************************************************/
/*************FUNCTION DEFINTION*****************/

return_type sdp_client_check(struct sockaddr_in *client_address)
{
	FILE *fp = NULL ;			/*file pointer*/
	char buffer[FIELD_SIZE]; 	/*to read from file*/
	char address[FIELD_SIZE];	/*to get client address*/

	TRACE("\nTRACE: Entering function sdp_client_check\n");
	/*Opening the file*/
	if(NULL == (fp = fopen("client_list" , "r")))
	{
		error_handling(ERR_MAJOR, SDP_FIELDS_FILE_ERROR,
						"sdp_client_check : fopen error.");
		TRACE("\nTRACE: Exiting function sdp_client_check with failure.\n");
		return FAILURE;
	}
	
	if(NULL == strncpy(address , inet_ntoa(client_address->sin_addr), FIELD_SIZE ))
	{
		error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR,
						"sdp_client_check : strncpy error.");
		TRACE("\nTRACE: Exiting function sdp_client_check with failure.\n");
		return FAILURE;
	}

	while(TRUE == TRUE)
	{
		memset(buffer , '\0' , FIELD_SIZE);
		if(NULL == fgets(buffer , FIELD_SIZE, fp))
		{
			break;
		}
		buffer[strlen(buffer) - 1] = '\0';

		if(!strncmp(address , buffer, strlen(buffer)))
		{
			if(EOF == fclose(fp))
			{
				error_handling(ERR_MAJOR, SDP_CLOSE_ERROR,
							"sdp_add_client : fclose error.");
				TRACE("\nTRACE: Exiting function sdp_client_check with failure.\n");
				return FAILURE;
			}
			TRACE("\nTRACE: Exiting function sdp_client_check with success.\n");
			return SUCCESS;
		}
		
	} // end of while
	
	if(EOF == fclose(fp))
	{
		error_handling(ERR_MAJOR, SDP_CLOSE_ERROR,
					"sdp_add_client : fclose error.");
		TRACE("\nTRACE: Exiting function sdp_client_check with failure.\n");
		return FAILURE;
	}
	TRACE("\nTRACE: Exiting function sdp_client_check with failure.\n");
	return FAILURE;
}
