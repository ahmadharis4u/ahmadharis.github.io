/**********************************************************************
*
* FILE NAME     : sdp_add_email.c
*
* DESCRIPTION   : To add the email field to session structure
*
*	DATE			NAME			REASON			REFERENCE
*
*	19-JAN-10	Ahmad Haris			Modfied				LLD
*
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
**********************************************************************/

/************HEADER FILES INCLUSION************/
#include"./../HEADER/sdp.h"



/**********************************************************************
*
* FUNCTION NAME : sdp_add_email
*
* DESCRIPTION   : To add the email field to session structure.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer, char pointer
*
***********************************************************************/


/************FUNCTION DEFINITION***************/

return_type sdp_add_email(sdp_session_t *sess,     /*session variable*/
                          char *file_name          /*file name*/
                         )
{
    FILE   *fp = NULL;                 /*file pointer*/
    char   temp_char = INIT;           /*for fgetc*/
    char   email_field[FILE_SIZE];            /*email_field buffer*/
    int    index = ZERO;               /*buffer index*/
    int    count = ZERO;               /*node count*/
	
	/*variables for assigning values to email field*/
    sdp_list_t   *ptr = NULL;
    sdp_list_t   *ptr_temp = NULL;

    TRACE("\nTRACE: Entering function sdp_add_email\n");

	memset(email_field, 0, FILE_SIZE);
    /**********OPENING THE INPUT FILE**********/

    fp = fopen(file_name,"r");
    /*error handling*/
    if(NULL == fp)
    {
		error_handling(ERR_MAJOR, SDP_EMAIL_FILE_ERROR, 
		       "sdp_add_email: File error ");
        TRACE("\nTRACE: Exiting function sdp_add_email with Failure\n");
 		return FAILURE;
    }

    while(TRUE == TRUE)
    {
        if ((int)(temp_char = (char)fgetc(fp)) == -1)
            break;

		if (temp_char == 'e')
		{
	    	(void)fgetc(fp);
	    	/*reading entire line*/
			for(index = 0, temp_char = (char)fgetc(fp); 
				temp_char != '\\'; 
				temp_char = (char)fgetc(fp), index++)
		   			email_field[index] = temp_char;
            
			email_field[index] = '\0';

	    	ptr = (sdp_list_t *) calloc(ONE, sizeof(sdp_list_t));
            
            if(NULL == ptr)
            {
				error_handling(ERR_MAJOR, SDP_EMAIL_MALLOC_ERROR,
				       "sdp_add_email: Malloc error ");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_email: fclose error ");
				}
                TRACE("\nTRACE: Exiting function sdp_add_email with Failure\n");
				return FAILURE;
            }
	    	ptr->value = (char *) calloc(strlen(email_field) + ONE,
							 sizeof(char));
            if(NULL == ptr->value)
            {
				error_handling(ERR_MAJOR, SDP_EMAIL_MALLOC_ERROR, 
			    	   "sdp_add_email: Malloc error ");
 				free(ptr);
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_email: fclose error ");
				}
                TRACE("\nTRACE: Exiting function sdp_add_email with Failure\n");
				return FAILURE;
            }
            if(NULL == strcpy(ptr->value, email_field))
	    	{
				error_handling(ERR_MAJOR, SDP_STRCPY_ERROR, 
			    	   "sdp_add_email: strcpy error ");
 				free(ptr);
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_email: fclose error ");
				}
                TRACE("\nTRACE: Exiting function sdp_add_email with Failure\n");
				return FAILURE;
            }
	    	ptr->next = NULL;

	    	if(count == 0)
	    	{
	        	sess->s_email = ptr;
				count++;
	    	}
	    	else
	    	{
				for(ptr_temp = sess->s_email; 
				ptr_temp->next != NULL;ptr_temp = ptr_temp->next);
				ptr_temp->next = ptr;
	    	}
		}//end of if

		for (temp_char = (char)fgetc(fp); temp_char != '\n'; 
			temp_char = (char)fgetc(fp));

    }//end of while
	
	if(EOF == fclose(fp))
	{
		error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
   			   	"sdp_add_email: fclose error ");
    	TRACE("\nTRACE: Exiting function sdp_add_email with Failure\n");
    	return FAILURE;
	}
    TRACE("\nTRACE: Exiting function sdp_add_email with Success\n");
    return SUCCESS;
}
