/**********************************************************************
*
* FILE NAME     : sdp_add_information.c
*
* DESCRIPTION   : To add the information field to session structure or
*                 media structure.
*
*	DATE			NAME			REASON			REFERENCE
*
*	19-JAN-10	Ahmad Haris,			Modfied				LLD
*				Deepshikha
*
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
**********************************************************************/

/**************HEADER FILES INCLUSION****************/
#include"./../HEADER/sdp.h"



/**********************************************************************
*
* FUNCTION NAME : sdp_add_information
*
* DESCRIPTION   : To add the information field to session structure or 
*                 media structure.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer, char pointer
*
***********************************************************************/
/**************FUNCTION DEFINITION*******************/

return_type sdp_add_information(sdp_session_t *sess,  /*session variable*/
                                char *file_name       /*file name*/
								)
{
    FILE   *fp = NULL;             /*file pointer*/
    char   temp_char = INIT;       /*for fgetc*/
    char   info_field[SIZE];       /*info_field buffer*/
    int    index = ZERO;           /*buffer index*/

    TRACE("\nTRACE: Entering function sdp_add_information\n");

	memset(info_field, 0, SIZE);

    /********OPENING THE INPUT FILE**********/
    fp = fopen(file_name, "r");
	/*error handling*/
    if(NULL == fp)
    {
		error_handling(ERR_MAJOR, SDP_INFO_FILE_ERROR,
		       "sdp_add_information: File error ");
        TRACE("\nTRACE: Exiting function sdp_add_information with Failure\n");
 		return FAILURE;
    }

    while(TRUE == TRUE)
    {
        temp_char = (char)fgetc(fp);

		if (temp_char == 'm')
        {
			if(EOF == fclose(fp))
			{
				error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    		   	"sdp_add_information: fclose error ");
          	  	TRACE("\nTRACE: Exiting function sdp_add_information with Failure\n");
				return FAILURE;
			}
    	    TRACE("\nTRACE: Exiting function sdp_add_information with success\n");
            return SUCCESS;
        }

        if (temp_char == 'i')
        {
            (char)fgetc(fp);
            for(index = 0, temp_char = (char)fgetc(fp); 
			temp_char != '\\'; temp_char = (char)fgetc(fp), index ++)
            	info_field[index] = temp_char;
            info_field[index] = '\0';

            sess->s_info = (char *) calloc(strlen(info_field) + ONE, 
							sizeof(char));
             
            if(NULL == sess->s_info)
            {
				error_handling(ERR_MAJOR, SDP_INFO_MALLOC_ERROR, 
				       "sdp_add_information: Malloc error ");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_information: fclose error ");
				}
    	        TRACE("\nTRACE: Exiting function sdp_add_information with Failure\n");
 				return FAILURE;
            }

            if(NULL == strncpy(sess->s_info, info_field,
				strlen(info_field) + 1))
	    	{

				error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
			    	   "sdp_add_information: strncpy error ");
				if(EOF == fclose(fp))
				{
					error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
		    			   	"sdp_add_information: fclose error ");
				}
    	        TRACE("\nTRACE: Exiting function sdp_add_information with Failure\n");
 				return FAILURE;
            }

        }//end of if

        for (temp_char = (char)fgetc(fp); temp_char != '\n';
		 	temp_char = (char)fgetc(fp));

    }//end of while
}//end of function
