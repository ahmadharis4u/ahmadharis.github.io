/**********************************************************************
*
* FILE NAME     : sdp_session_to_str.c
*
* DESCRIPTION   : To convert the session structure to string.
*
* Date          Name        Reason              Reference                
*
* 19-Jan-10     Neeraj      Code Review         LLD
*
* Copyright(C) 2010 Aricent Inc. All Rights Reserved.
*
*********************************************************************/

/**************HEADER FILE DECALARATIONS************/
#include"./../HEADER/sdp.h"



/***********************************************************************

 FUNCTION NAME : sdp_session_to_str

 DESCRIPTION   : To convert the session to string.

 RETURN TYPE   : character pointer

 PARAMETERS    : sdp_session_t pointer and integer pointer

**********************************************************************/
char * sdp_session_to_str(sdp_session_t *sess, 	/*session variable*/
		            int *error		/*error number*/
			  )
{
    /* SDP pointer variables */
   sdp_list_t       *ptr_list = NULL;
   sdp_conn_t       *ptr_conn = NULL;
   sdp_bandwidth_t  *ptr_bw = NULL;
   sdp_zone_t       *ptr_zone = NULL;
   sdp_attr_t       *ptr_attr = NULL;
   sdp_media_t      *ptr_media = NULL;
   
   char             *str = NULL;    /* pointer to SDP message string */
   char             *temp = NULL;   /* temporary string pointer variable*/
   char             attr_buf[ATTR_SIZE]; /* buffer to store
                                            attributes*/
   char             field_buf[FIELD_SIZE]; /* buffer to store field*/
   int              size = ZERO;           /* data size variable*/
   int              mem_count = 2;         /* memory counter */
   
   *error = ERR_ALL;
   TRACE("\nTRACE: Entering function sdp_session_to str\n");
   
   /* Initialize char arrays */
   memset(attr_buf, 0, ATTR_SIZE);
   memset(field_buf, 0, FIELD_SIZE);

   str = (char *) malloc(BLOCK + MARGIN);

   if(str ==NULL)
   {
       error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR, 
			"sdp_session_to_str: Malloc Error ");
       TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
       return NULL;
   }

   /***Adding version attribute to string***/
   (void)snprintf(attr_buf,(size_t) sizeof(int), "%d", sess->s_version);
   size = (int)strlen(attr_buf) + 2;

   if(NULL == strncpy(str, "v=", 1 + strlen("v=")))
   {
       error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
			"sdp_session_to_str: strncpy Error ");
       TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
       return NULL;
   }
	
   strcat(str, attr_buf);
   strcat(str, ";");

   /***Adding orgin attribute to string***/

   if(NULL == strncpy(attr_buf, sess->s_origin->o_username, 
                      1 + strlen(sess->s_origin->o_username)))
   {
       error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
			"sdp_session_to_str: strncpy Error ");
       TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
       return NULL;
   }

   (void)snprintf(field_buf,(size_t)sizeof(long) ,"%lu", sess->s_origin->o_id);
   strcat(attr_buf, " ");
   strcat(attr_buf, field_buf);
   (void)snprintf(field_buf,(size_t)sizeof(long) ,"%lu",
   					sess->s_origin->o_version);
   strcat(attr_buf, " ");
   strcat(attr_buf, field_buf);
   strcat(attr_buf, " ");
   strcat(attr_buf, sess->s_origin->o_nettype);
   strcat(attr_buf, " ");
   strcat(attr_buf, sess->s_origin->o_addrtype);
   strcat(attr_buf, " ");
   strcat(attr_buf, sess->s_origin->o_address);
   size += (int)(strlen(attr_buf) + 2);

   /***module since realloc may fail***/

   if(size > BLOCK)
   {
       temp = (char *) malloc(sizeof(char) * (mem_count * BLOCK + MARGIN));
       if(temp ==NULL)
       {
           error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR,
		   					"sdp_session_to_str: Malloc Error ");
       	   TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
           return NULL;
       }
       if(NULL == strncpy(temp, str, 1 + strlen(str)))
       {
       	    error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
						"sdp_session_to_str: strncpy Error ");
            free(str);
      	    TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
      	    return NULL;
   	}
        free(str);
        str = temp;
        mem_count++;
        size -= BLOCK;

   }//end of if

   strcat(str, "o=");
   strcat(str, attr_buf);
   strcat(str, ";");

   /***Adding name attribute to string***/
   if(NULL == strncpy(attr_buf, sess->s_name, 1 + strlen(sess->s_name)))
   {
       error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
			"sdp_session_to_str: strncpy Error ");
       TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
       return NULL;
   }
   size += (strlen(attr_buf) + 2);

   if(size > BLOCK)
   {
       temp = (char *) malloc(sizeof(char) * (mem_count * BLOCK + MARGIN));

       if(temp ==NULL)
       {
           error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR,
		   					"sdp_session_to_str: Malloc Error ");
           TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
           return NULL;
       }

       if(NULL ==strncpy(temp, str, 1 + strlen(str)))
       {
      	    error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
						"sdp_session_to_str: strncpy Error ");
            free(str);
            TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
            return NULL;
	}
    	free(str);
       	str = temp;
       	mem_count++;
       	size -= BLOCK;
   }
   strcat(str, "s=");
   strcat(str, attr_buf);
   strcat(str, ";");

   /***Adding information attribute to string***/
   if(NULL != sess->s_info)
   {
        if(NULL==strncpy(attr_buf, sess->s_info, 1 + strlen(sess->s_info)))
        {
      	    error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
	    				"sdp_session_to_str: strncpy Error ");
            TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
            return NULL;
        }
        size += (strlen(attr_buf) + 2);
   
	if(size > BLOCK)
   	{
       	    temp = (char *) malloc(sizeof(char) * (mem_count * BLOCK + MARGIN));
       	    if(temp ==NULL)
       	    {
           	error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR,
								"sdp_session_to_str: Malloc Error ");
       	   	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
           	return NULL;
       	    }
       	    if(NULL == strncpy(temp, str, 1 + strlen(str)))
	    {
       		error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
					"sdp_session_to_str: strncpy Error ");
                free(str);
       		TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
    		return NULL;
   	    }
       	    free(str);
       	    str = temp;
       	    mem_count++;
       	    size -= BLOCK;
   	}
       	strcat(str, "i=");
       	strcat(str, attr_buf);
       	strcat(str, ";");
   }

   /***Adding uri attribute to string***/
    if(NULL != sess->s_uri)
    {
        if(NULL== strncpy(attr_buf, sess->s_uri, 1 + strlen(sess->s_uri)))
        {
	
      	    error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
				"sdp_session_to_str: strncpy Error ");
            TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
            return NULL;
       	}
       	size += (strlen(attr_buf) + 2);

       	if(size > BLOCK)
       	{
            temp = (char *) malloc(sizeof(char) * (mem_count * BLOCK + MARGIN));
            if(temp ==NULL)
            {
               	error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR, 
									"sdp_session_to_str: Malloc Error ");
              	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
            	return NULL;
            }
            if(NULL== strncpy(temp, str, 1 + strlen(str)))
	    {
		
      	 	error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
		    			"sdp_session_to_str: strncpy Error ");
		free(str);
         	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
        	return NULL;
            }
            free(str);
            str = temp;
            mem_count++;
            size -= BLOCK;
       }
       strcat(str, "u=");
       strcat(str, attr_buf);
       strcat(str, ";");
   }

   /***Adding email attribute to string***/
   ptr_list = sess->s_email;
   if(NULL != sess->s_email)
   
    while(TRUE == TRUE)
    {
        if(NULL == strncpy(attr_buf, ptr_list->value, 1 + strlen(ptr_list->value)))
	{
		
      	   error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
	    			"sdp_session_to_str: strncpy Error ");
           TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
           return NULL;
        }
        
       	size += (strlen(attr_buf) + 2);
   	if(size > BLOCK)
   	{
       	    temp = (char *) malloc(sizeof(char) * (mem_count * BLOCK + MARGIN));
       	    if(temp ==NULL)
       	    {
           	error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR, 
	    				"sdp_session_to_str: Malloc Error ");
           	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
           	return NULL;
       	    }
       	    if(NULL == strncpy(temp, str, 1 + strlen(str)))
	    {
			
      	        error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
					"sdp_session_to_str: strncpy Error ");
		free(str);
         	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
         	return NULL;
            }
       	    free(str);
       	    str = temp;
       	    mem_count++;
       	    size -= BLOCK;
   	}
       	strcat(str, "e=");
       	strcat(str, attr_buf);
       	strcat(str, ";");
       	if(NULL == ptr_list->next)
       	    break;
       	else
       	    ptr_list = ptr_list->next;
    }
   
    /***Adding phone attribute to string***/
    ptr_list = sess->s_phone;
    if(NULL != sess->s_phone)
    while(TRUE == TRUE)
    {
       	if( NULL == strncpy(attr_buf, ptr_list->value, 1 + strlen(ptr_list->value)))
	{
		
      	    error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
							"sdp_session_to_str: strncpy Error ");
            TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
            return NULL;
        }
       	size += (strlen(attr_buf) + 2);
   	if(size > BLOCK)
   	{
       	    temp = (char *) malloc(sizeof(char) * (mem_count * BLOCK + MARGIN));
       	    if(temp ==NULL)
       	    {
           	error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR, 
								"sdp_session_to_str: Malloc Error ");
       	   	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
           	return NULL;
       	    }
       	    if(NULL == strncpy(temp, str, 1 + strlen(str)))
	    {
			
      	 	error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
    					"sdp_session_to_str: strncpy Error ");
		free(str);
         	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
        	return NULL;
            }
       	    free(str);
       	    str = temp;

            mem_count++;
            size -= BLOCK;
    	}
        strcat(str, "p=");
        strcat(str, attr_buf);
        strcat(str, ";");
        if(NULL == ptr_list->next)
            break;
        else
            ptr_list = ptr_list->next;
    }//end of while

    /***Adding Connection attribute to string***/
    ptr_conn = sess->s_conn;
    if(NULL != sess->s_conn)
    while(TRUE == TRUE)
    {
        if(NULL== strncpy(attr_buf, ptr_conn->c_nettype, 1 + strlen(ptr_conn->c_nettype)))
	{
		error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
		    		"sdp_session_to_str: strncpy Error ");
        	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
       	  	return NULL;
        }
        strcat(attr_buf, " ");
        strcat(attr_buf, ptr_conn->c_addrtype);
        strcat(attr_buf, " ");
        strcat(attr_buf, ptr_conn->c_address);
	if(ptr_conn->c_ttl != 0)
	{
            strcat(attr_buf, "/");
            (void)snprintf(field_buf,(size_t)sizeof(int) ,"%u", ptr_conn->c_ttl);
            strcat(attr_buf, field_buf);
            strcat(attr_buf, "/");
            (void)snprintf(field_buf,(size_t)sizeof(int),"%d", ptr_conn->c_addrcount);
            strcat(attr_buf, field_buf);
	}
        size += (strlen(attr_buf) + 2);
    	
	if(size > BLOCK)
    	{
            temp = (char *) malloc(sizeof(char) * (mem_count * BLOCK + MARGIN));
       	    if(temp ==NULL)
       	    {
                error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR,
				    "sdp_session_to_str: Malloc Error ");
           	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
           	return NULL;
       	    }
			
            if(NULL == strncpy(temp, str, 1 + strlen(str)))
	    {
      	 	error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
				    "sdp_session_to_str: strncpy Error ");
		free(str);
        	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
         	return NULL;
            }
            free(str);
            str = temp;
            mem_count++;
            size -= BLOCK;
    	}
        strcat(str, "c=");
        strcat(str, attr_buf);
        strcat(str, ";");
        if(NULL == ptr_conn->c_next)
            break;
        else
        ptr_conn = ptr_conn->c_next;
    }//end of while

    /***Adding bandwidth attribute to string***/
    ptr_bw = sess->s_bw;
    if(NULL != ptr_bw)
    while(TRUE == TRUE)
    {
        if(NULL == strncpy(attr_buf, ptr_bw->b_type, 1 + strlen(ptr_bw->b_type)))
	{	
      	    error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
				"sdp_session_to_str: strncpy Error ");
            TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
            return NULL;
        }
        strcat(attr_buf, ":");
        (void)snprintf(field_buf, (size_t)sizeof(int),"%u", ptr_bw->b_value);
        strcat(attr_buf, field_buf);
        size += (strlen(attr_buf) + 2);
    	if(size > BLOCK)
    	{
            temp = (char *) malloc(sizeof(char) * (mem_count * BLOCK + MARGIN));
       	    if(temp ==NULL)
       	    {
           	error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR,
					"sdp_session_to_str: Malloc Error ");
           	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
           	return NULL;
       	    }
            if(NULL == strncpy(temp, str, 1 + strlen(str)))
	    {    	
      	 	error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
				    "sdp_session_to_str: strncpy Error ");
		free(str);
        	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
      		return NULL;
            }
            free(str);
            str = temp;
            mem_count++;
            size -= BLOCK;
    	}
        strcat(str, "b=");
        strcat(str, attr_buf);
        strcat(str, ";");
        if(NULL == ptr_bw->b_next)
            break;
        else
            ptr_bw = ptr_bw->b_next;

    }//end of while


    /***Adding time attribute to string***/
    if(NULL != sess->s_time)
    {
        (void)snprintf(field_buf,(size_t)sizeof(long) ,
						"%lu", sess->s_time->t_start);
        if(NULL == strncpy(attr_buf, field_buf, 1 + strlen(field_buf)))
	{
		
      	   error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
				"sdp_session_to_str: strncpy Error ");
           TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
           return NULL;
        }
        strcat(attr_buf, " ");
        (void)snprintf(field_buf,(size_t)sizeof(long),"%lu", sess->s_time->t_stop);
        strcat(attr_buf, field_buf);
        size += (strlen(attr_buf) + 2);
        if(size > BLOCK)
        {
            temp = (char *) malloc(sizeof(char) * (mem_count * BLOCK + MARGIN));
       	    if(temp ==NULL)
            {
                error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR, "sdp_session_to_str: Malloc Error ");
       	        TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
                return NULL;
            }
            if(NULL == strncpy(temp, str, 1 + strlen(str)))
	    {    
      	 	error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
				    "sdp_session_to_str: strncpy Error ");
		free(str);
      		TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
         	return NULL;
            }
            free(str);
            str = temp;
            mem_count++;
            size -= BLOCK;
        }
        strcat(str, "t=");
        strcat(str, attr_buf);
        strcat(str, ";");
    } /* End of if */
    /***Adding repeat attribute to string***/
    if(sess->s_time != NULL)
    if(NULL != sess->s_time->t_repeat)
    {
        (void)snprintf(field_buf,(size_t)sizeof(long),
						"%lu", sess->s_time->t_repeat->r_interval);
        if(NULL== strncpy(attr_buf, field_buf, 1 + strlen(field_buf)))
	{	
      	    error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
				"sdp_session_to_str: strncpy Error ");
            TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
            return NULL;
        }
        strcat(attr_buf, " ");
        (void)snprintf(field_buf,(size_t)sizeof(long),
					"%lu",sess->s_time->t_repeat->r_duration);
        strcat(attr_buf, field_buf);
        ptr_list = sess->s_time->t_repeat->r_offset;
        while(TRUE == TRUE)
        {
            strcat(attr_buf, " ");
            strcat(attr_buf, ptr_list->value);
            if(ptr_list->next == NULL)
                break;
            else
                ptr_list = ptr_list->next;

        }
        size += (strlen(attr_buf) + 2);
        if(size > BLOCK)
        {
            temp = (char *) malloc(sizeof(char) * (mem_count * BLOCK + MARGIN));
            if(temp ==NULL)
            {
               	error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR,
									"sdp_session_to_str: Malloc Error ");
  	       	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
               	return NULL;
            }
            if(NULL == strncpy(temp, str, strlen(str)))
	    {

      	        error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
   		    		    "sdp_session_to_str: strncpy Error ");
	       	free(str);
               	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
               	return NULL;
            }
            free(str);
            str = temp;
            mem_count++;
            size -= BLOCK;
        }
        strcat(str, "r=");
        strcat(str, attr_buf);
        strcat(str, ";");
    }//end of while

    /***Adding zone attribute to string***/
    ptr_zone = sess->s_zone;
    if(NULL == strncpy(attr_buf, "", ONE))
    {

      	 error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
				"sdp_session_to_str: strncpy Error ");
         TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
         return NULL;
    }
    if(NULL != ptr_zone)
    {
        while(TRUE == TRUE)
        {
            (void)snprintf(field_buf, (size_t)sizeof(long),
							"%lu", ptr_zone->z_time);
            strcat(attr_buf, field_buf);
            strcat(attr_buf, " ");
            strcat(attr_buf, ptr_zone->z_offset);
            if(NULL == ptr_zone->z_next)
                break;
            else
            {
                strcat(attr_buf, " ");
                ptr_zone = ptr_zone->z_next;
            }
        }
        size += (strlen(attr_buf) + 2);
        if(size > BLOCK)
        {
            temp = (char *) malloc(sizeof(char) * (mem_count * BLOCK + MARGIN));
            if(temp ==NULL)
            {
                error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR,
								"sdp_session_to_str: Malloc Error ");
       	       	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
               	return NULL;
            }
            if(NULL == strncpy(temp, str, 1 + strlen(str)))
	    {

      	       error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
	   	    			  "sdp_session_to_str: strncpy Error ");
               free(str);
               TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
               return NULL;
            }
            free(str);
            str = temp;
            mem_count++;
            size -= BLOCK;
        }
        strcat(str, "z=");
        strcat(str, attr_buf);
        strcat(str, ";");
    }

    /***Adding key attribute to string***/
    if(NULL != sess->s_key)
    {
        if(NULL == strncpy(attr_buf, sess->s_key->k_method, 1 + strlen(sess->s_key->k_method)))
        {
      	    error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
	    	   			"sdp_session_to_str: strncpy Error ");
            TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
            return NULL;
        }
        size += (strlen(attr_buf) + 2);
        if(size > BLOCK)
        {
            temp = (char *) malloc(sizeof(char) * (mem_count * BLOCK + MARGIN));
            if(temp ==NULL)
            {
                error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR, 
								"sdp_session_to_str: Malloc Error ");
               	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
               	return NULL;
            }
            if(NULL == strncpy(temp, str, 1 + strlen(str)))
	    	{

      	    	error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
		        		    "sdp_session_to_str: strncpy Error ");
	   	free(str);
              	TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
              	return NULL;
            }
            free(str);
            str = temp;
            mem_count++;
            size -= BLOCK;
        }
        strcat(str, "k=");
        strcat(str, attr_buf);
        strcat(str, ";");
    }//end of while

    /***Adding attribute field to string***/
    if(NULL != sess->s_attr)
    {
        if(NULL == strncpy(attr_buf, sess->s_attr->a_name, 1 + strlen(sess->s_attr->a_name)))
	{
		
      	    error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
						"sdp_session_to_str: strncpy Error ");
            TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
            return NULL;
        }
        size += (strlen(attr_buf) + 2);
        if(size > BLOCK)
        {
            temp = (char *) malloc(sizeof(char) * (mem_count * BLOCK + MARGIN));
            if(temp ==NULL)
            {
                error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR, 
								"sdp_session_to_str: Malloc Error ");
       	       TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
               return NULL;
            }
            if(NULL == strncpy(temp, str, 1 + strlen(str)))
	    {

      	      error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
							"sdp_session_to_str: strncpy Error ");
              free(str);
              TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
              return NULL;
            }
            free(str);
            str = temp;
            mem_count++;
            size -= BLOCK;
        }
        strcat(str, "a=");
        strcat(str, attr_buf);
        //strcat(str, ";");
	
    }
    
	
    /***Adding media attributes to string***/
    ptr_media = sess->s_media;
    if(NULL != ptr_media)
    while(TRUE == TRUE)
    {
        if(NULL == strncpy(attr_buf, "m=", 1 + strlen("m=")))
	{

      	   error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
			  	"sdp_session_to_str: strncpy Error ");
           TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
           return NULL;
        }
        strcat(attr_buf, ptr_media->m_name);
        strcat(attr_buf, " ");
        (void)snprintf(field_buf, (size_t)sizeof(int),"%d", ptr_media->m_port);
        strcat(attr_buf, field_buf);
        strcat(attr_buf, " ");
        (void)snprintf(field_buf, (size_t)sizeof(int),"%d", ptr_media->m_portcount);
        strcat(attr_buf, field_buf);
        strcat(attr_buf, " ");
        strcat(attr_buf, ptr_media->m_proto);
        ptr_list = ptr_media->m_format;
        while(TRUE == TRUE)
        {
            strcat(attr_buf, " ");
            strcat(attr_buf, ptr_list->value);
            if(NULL == ptr_list->next)
                break;
            else
                ptr_list = ptr_list->next;
        }
    	if(NULL != ptr_media->m_info)
    	{
            strcat(attr_buf, ";");
            strcat(attr_buf, "i=");
            strcat(attr_buf, ptr_media->m_info);
            strcat(attr_buf, ";");
    	}
    	if(NULL != ptr_media->m_conn)
    	{
            strcat(attr_buf, "c=");
            strcat(attr_buf, ptr_media->m_conn->c_nettype);
            strcat(attr_buf, " ");
            strcat(attr_buf, ptr_media->m_conn->c_addrtype);
            strcat(attr_buf, " ");
            strcat(attr_buf, ptr_media->m_conn->c_address);
	    
            if(ptr_media->m_conn->c_ttl != 0)
	    {
       		strcat(attr_buf, "/");
       		(void)snprintf(field_buf,(size_t)sizeof(int) ,
							"%u", ptr_media->m_conn->c_ttl);
       		strcat(attr_buf, field_buf);
       		strcat(attr_buf, "/");
       		(void)snprintf(field_buf,(size_t)sizeof(int),
							"%d", ptr_media->m_conn->c_addrcount);
       		strcat(attr_buf, field_buf);
	    }
        	strcat(attr_buf, ";");
    	}
    	if(NULL != ptr_media->m_bw)
    	{
       	    strcat(attr_buf, "b=");
            strcat(attr_buf, ptr_media->m_bw->b_type);
            strcat(attr_buf, ":");
            (void)snprintf(field_buf, (size_t)sizeof(int),
					    "%u", ptr_media->m_bw->b_value);
            strcat(attr_buf, field_buf);
            strcat(attr_buf, ";");
    	}

    	if(NULL != ptr_media->m_key)
    	{
            strcat(attr_buf, "k=");
            strcat(attr_buf, ptr_media->m_key->k_method);
            strcat(str, ";");
    	}
    	ptr_attr = ptr_media->m_attr;
    	if(NULL != ptr_attr)
    	while(TRUE == TRUE)
    	{
            strcat(attr_buf, "a=");
            strcat(attr_buf, ptr_attr->a_name);
            strcat(attr_buf, " ");
            strcat(attr_buf, ptr_attr->a_value);
            strcat(attr_buf, ";");
            if(NULL == ptr_attr->a_next)
            	break;
            else
     	       	ptr_attr = ptr_attr->a_next;
    	}
        size += (strlen(attr_buf) + 2);

 	if(size > BLOCK)
    	{
            temp = (char *) malloc(sizeof(char) * (mem_count * BLOCK + MARGIN));
            if(temp ==NULL)
            {
            	error_handling(ERR_MAJOR, SDP_STRING_MALLOC_ERROR, 
				    "sdp_session_to_str: Malloc Error ");
           		TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
           		return NULL;
        	}
        	if(NULL == strncpy(temp, str, 1 + strlen(str)))
		{	
      	    	    error_handling(ERR_MAJOR, SDP_STRNCPY_ERROR, 
		         "sdp_session_to_str: strncpy Error ");
		   		free(str);
        	    TRACE("\nTRACE: Exiting function sdp_session_to_str with failure\n");
         	    return NULL;
        	}
        	free(str);
        	str = temp;
        	mem_count++;
        	size -= BLOCK;
    	}
        strcat(str,attr_buf);
        strcat(str, ";");
        if(NULL == ptr_media->m_next)
        {
            //strcat(str, "$#$");
            break;
        }
        else
            ptr_media= ptr_media->m_next;
    } /*End of while*/
    
    TRACE("\nTRACE: Exiting function sdp_session_to_str with success\n");
    
    return(str);
}
