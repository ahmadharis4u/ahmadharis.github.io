
/**********************************************************************
* 
* FILE NAME     : sdp_add_attribute.c
*
* DESCRIPTION   : To add the attribute field to session structure or 
*		  media structure.
*
*	DATE			NAME			REASON			REFERENCE
*
*	19-JAN-10	Ahmad Haris			Modfied				LLD
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
**********************************************************************/

/**************HEADER FILE DECALARATIONS************/

#include"./../HEADER/sdp.h"



/**********************************************************************
* 
* FUNCTION NAME : sdp_add_attribute
*
* DESCRIPTION   : To add the attribute field to session structure or 
*		  media structure.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer, char pointer
*
**********************************************************************/

/**************FUNCTION DEFINITION******************/

return_type sdp_add_attribute(sdp_session_t *sess,    /* session pointer*/
                              char *file_name         /* file name*/
                              )
{
    FILE   *fp = NULL;             /*file pointer*/
    char   temp_char = INIT;       /*for fgetc*/
    char   attr_field[SIZE];       /*attr_field buffer*/
    int    index = ZERO;           /*buffer index*/

    TRACE("\nTRACE: Entering function sdp_add_attribute\n"); 
   
   	memset(attr_field, 0, SIZE);
    
	/*********OPENING THE INPUT FILE********/
    fp = fopen(file_name, "r");
	/*error handling*/
    if(NULL == fp)
    {
        error_handling(ERR_MAJOR, SDP_ATTRIBUTE_FILE_ERROR,
		       "sdp_add_attribute: File error ");
        
        TRACE("\nTRACE: Exiting function sdp_add_attribute with Failure\n"); 
		return FAILURE;
    }
    
    while(TRUE == TRUE)
    {
        temp_char = (char)fgetc(fp);

		if(temp_char == 'm')
        {
			if(EOF == fclose(fp))
			{
				error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    		   	"sdp_add_attribute: fclose error ");
          	  	TRACE("\nTRACE: Exiting function sdp_add_attribute with Failure\n");
				return FAILURE;
			}
            TRACE("\nTRACE: Exiting function sdp_add_attribute with Success\n");
			return SUCCESS;
        }

        if(temp_char == 'a')
        {
            (void)fgetc(fp);
	    
	    /***This for loop will read attribute until '\' is encountered***/

            for(index = 0, temp_char = (char)fgetc(fp);                                          temp_char != '\\'; temp_char = (char)fgetc(fp), index ++)
                attr_field[index] = temp_char;

            attr_field[index] = '\0';

	    /*******ASSINGNING THE VALUES TO ATTRIBUTE STRUCTURE****/
		    sess->s_attr = (sdp_attr_t *) calloc(ONE, sizeof(sdp_attr_t));

		    if(NULL == sess->s_attr)
            {
				error_handling(ERR_MAJOR, SDP_ATTRIBUTE_MALLOC_ERROR, 
			    		   "sdp_add_attribute: Malloc Error ");	

				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_attribute: fclose error ");
				}
                TRACE("\nTRACE: Exiting function sdp_add_attribute with Failure\n"); 
				return FAILURE;
            }

            sess->s_attr->a_name = (char *) calloc(strlen(attr_field) + ONE,
								 sizeof(char));

	    	if(NULL == sess->s_attr->a_name)
            {
				error_handling(ERR_MAJOR, SDP_ATTRIBUTE_MALLOC_ERROR,
					       "sdp_add_attribute: Malloc Error ");

				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_attribute: fclose error ");
				}
                TRACE("\nTRACE: Exiting function sdp_add_attribute with Failure\n"); 
				return FAILURE;
            }

            if (NULL == strncpy(sess->s_attr->a_name, attr_field, 
				strlen(attr_field) + 1))
	    	{
		
				error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR,
			    		   "sdp_add_attribute: strncpy error ");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_attribute: fclose error ");
				}
                TRACE("\nTRACE: Exiting function sdp_add_attribute with Failure\n"); 
				return FAILURE;
	    	}

	    	sess->s_attr->a_value = NULL;
	    	sess->s_attr->a_next = NULL;
	
        }
	
	
	/*This for loop will move the pointer to the starting of the next line of the input file***/

		for(temp_char = (char)fgetc(fp); temp_char != '\n'; 
			temp_char = (char)fgetc(fp));

    }// end of while loop

}// end of function
