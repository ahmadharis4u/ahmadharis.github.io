/**********************************************************************
*
* FILE NAME : sdp_add_version
*
* DESCRIPTION   : To add the version number of the session.
*
*
*	DATE			NAME			REASON			REFERENCE
*
*	19-JAN-10	Ahmad Haris,		Modfied				LLD
*				Deepshikha
*
* Copyright @ 2010 Aricent Inc. All Rights Reserved.
*
***********************************************************************/

/*************HEADER FILES INCLUSION**************/
#include"./../HEADER/sdp.h"


/**********************************************************************
i*
* FUNCTION NAME : sdp_add_version
*
* DESCRIPTION   : To add the version number of the session.
*
* RETURN TYPE   : return_type
*
* PARAMETERS    : sdp_session_t pointer
*
***********************************************************************/
return_type sdp_add_version(sdp_session_t *sess,    /*session variable*/
                            char *file_name         /*file name*/
							)
{
    FILE   *fp = NULL;            /*file pointer*/
    char   temp_char = INIT;      /*for fgetc*/
    char   version_field[SIZE];   /*version_field buffer*/
    int    index = ZERO;          /*buffer index*/
	
    TRACE("\nTRACE: Entering function sdp_add_version\n");

	memset(version_field, 0, SIZE);
  /******OPENING THE INPUT FILE******/
    fp = fopen(file_name, "r");
    /*error handling*/
	if(NULL == fp)
    {
        error_handling(ERR_MAJOR,SDP_VERSION_FILE_ERROR,
			" sdp_add_version: File error ");
        TRACE("\nTRACE: Exiting function sdp_add_version with failure\n");
		return FAILURE;
    }
    /*reading the file*/
	while(TRUE == TRUE)
    {
		temp_char = (char)fgetc(fp);
		/*checking for version*/
		if(temp_char == 'v')
		{
	    	(void)fgetc(fp);
	    
	/***Assign version attribute to session structure***/    
	    	for(index = 0, temp_char = (char)fgetc(fp); 
				temp_char != '\\'; 
				temp_char = (char)fgetc(fp), index ++)
	    	{
				version_field[index] = temp_char;
	    	}
            version_field[index] = '\0';
            sess->s_version = atoi(version_field);
	    	break;
		}
    }
	if(EOF == fclose(fp))
	{
		error_handling(ERR_MINOR, SDP_CLOSE_ERROR,
    		   	"sdp_add_version: fclose error ");
       	TRACE("\nTRACE: Exiting function sdp_add_version with Failure\n");
		return FAILURE;
	}
    TRACE("\nTRACE: Exiting function sdp_add_version with success\n");
    return SUCCESS;
}
