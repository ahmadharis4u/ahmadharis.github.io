/***********************************************************************
*
* FILE NAME     : sdp_add_phone.c
*
* DESCRIPTION   : To add the phone field to session structure.
*
*
*	DATE			NAME			REASON			REFERENCE
*
*	19-JAN-10	Ahmad Haris			Modfied				LLD
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
*
***********************************************************************/

/********************HEADER FILE INCLUSION**************/
#include"./../HEADER/sdp.h"


/*********************************************************************
*
* FUNCTION NAME : sdp_add_phone
*
* DESCRIPTION   : To add the zone field to session structure.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer, char pointer
*
**********************************************************************/
return_type sdp_add_phone(sdp_session_t *sess,  /*session variable*/
                          char *file_name       /*file name*/
						  )
{
    FILE   *fp = NULL;          /*file pointer*/
    char   temp_char = INIT;    /*for fgetc*/
    char   phone_field[SIZE];   /*phone_field buffer*/
    int    index = ZERO;        /*buffer index*/
    int    count = ZERO;        /*node count*/
    
	/*sdp_list_t pointers to store phone info*/
	sdp_list_t 	*ptr = NULL,
				*ptr_temp = NULL;

    TRACE("\nTRACE: Entering function sdp_add_phone\n");	

	memset(phone_field, 0, SIZE);
    /****OPENING THE INPUT FILE*****/
    fp = fopen(file_name,"r");
    /*error handling*/
    if(NULL == fp)
    {
        error_handling(ERR_MAJOR,SDP_PHONE_FILE_ERROR,
			"sdp_add_phone: File error");
        TRACE("\nTRACE: Exiting function sdp_add_phone with Failure\n");	
		return FAILURE;
    }
   /*reading the file*/ 
    while(TRUE == TRUE)
    {
        if((int)(temp_char = (char)fgetc(fp)) == -1)
            break;
	/*checking phone field*/	
	if(temp_char == 'p')
	{
	    (void)fgetc(fp);
	    
        /***Assign phone attributes to session structure***/ 
	    for(index = 0, temp_char = (char)fgetc(fp); 
	        temp_char != '\\'; 
			temp_char = (char)fgetc(fp),index++)
	    {
			phone_field[index] = temp_char;
	    }
        
		phone_field[index] = '\0';
	    /*going to the last node*/
		if(count == ZERO)
	    {
	        sess->s_phone = (sdp_list_t *) calloc(ONE, sizeof(sdp_list_t));
		
			if(NULL == sess->s_phone)
			{
		    	error_handling(ERR_MAJOR,SDP_PHONE_MALLOC_ERROR,
					"sdp_add_phone: Malloc Error");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_phone: fclose error ");
				}
            	TRACE("\nTRACE: Exiting function sdp_add_phone with Failure\n");
		    	return FAILURE;
			}    
	        sess->s_phone->value = (char *) calloc(strlen(phone_field) + ONE,
													sizeof(char));
			if(NULL == sess->s_phone->value)
			{
		    	error_handling(ERR_MAJOR,SDP_PHONE_MALLOC_ERROR,
						"sdp_add_phone: Malloc error");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_phone: fclose error ");
				}
            	TRACE("\nTRACE: Exiting function sdp_add_phone with Failure\n");	
		    	return FAILURE;
			}    
        	if (NULL == (strncpy(sess->s_phone->value, phone_field,
				strlen(phone_field) + 1)))
			{
		    	error_handling(ERR_MAJOR,SDP_STRNCPY_ERROR,
						"sdp_add_phone: Strncpy error");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_phone: fclose error ");
				}
            	TRACE("\nTRACE: Exiting function sdp_add_phone with Failure\n");
		    	return FAILURE;
			}
			sess->s_phone->next = NULL;
			count++;
	    }
	    else /*for count != 0*/
	    {
	        ptr = (sdp_list_t *) calloc(ONE, sizeof(sdp_list_t));
			/*error handling*/
			if(NULL == ptr)
			{
		    	error_handling(ERR_MAJOR,SDP_PHONE_MALLOC_ERROR,
					   "sdp_add_phone: Malloc error");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_phone: fclose error ");
				}
            	TRACE("\nTRACE: Exiting function sdp_add_phone with Failure\n");
		    	return FAILURE;
			}    
	        ptr->value = (char *) malloc(strlen(phone_field) + ONE);
			/*error handling*/
			if(NULL == ptr->value)
			{
		    	error_handling(ERR_MAJOR,SDP_PHONE_MALLOC_ERROR,
						"sdp_add_phone:Malloc error");
            	free(ptr);
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_phone: fclose error ");
				}
		    	TRACE("\nTRACE: Exiting function sdp_add_phone with Failure\n");
		   	 	return FAILURE;
		    
			}    
			/*copying phone_field*/
        	if (NULL == (strncpy(ptr->value, phone_field,
				strlen(phone_field) + 1)))
			{
		    	error_handling(ERR_MAJOR,SDP_STRNCPY_ERROR,
							"sdp_add_phone: Strncpy error");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_phone: fclose error ");
				}
            	TRACE("\nTRACE: Exiting function sdp_add_phone with Failure\n");
		    	return FAILURE;
			}
			ptr->next = NULL;
			/*going to last node*/
			for(ptr_temp = sess->s_phone; 
		    	ptr_temp->next != NULL; 
		    	ptr_temp = ptr_temp->next);
		    
			ptr_temp->next = ptr; /*adding new node at the end*/
	    
	    }
	}
	/*checking for '\n'*/
	for(temp_char = (char)fgetc(fp); 
	    temp_char != '\n'; 
	    temp_char = (char)fgetc(fp));
    } /* end of while */
	
	if(EOF == fclose(fp))
	{
		error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
    			   	"sdp_add_phone: fclose error ");
 	   	TRACE("\nTRACE: Exiting function sdp_add_phone with Failure\n");	
    	return FAILURE;
	}
    TRACE("\nTRACE: Exiting function sdp_add_phone with Success\n");	
    return SUCCESS;
}
