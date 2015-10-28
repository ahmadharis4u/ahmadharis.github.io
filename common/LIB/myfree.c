/**********************************************************************
*
* FILE NAME     : myfree.c
*
* DESCRIPTION   : To free memory allocated to all session attributes
*
* Date          Name        Reason              Reference                
*
* 19-Jan-10     Neeraj      Code Review         LLD
*
* Copyright(C) 2010 Aricent Inc. All Rights Reserved.
**********************************************************************/

/**************HEADER FILE DECALARATIONS************/

#include"./../HEADER/sdp.h"


/***********************************************************************
*
* FUNCTION NAME : myfree
*
* DESCRIPTION   : To free memory allocated to all session attributes
*
* RETURN TYPE   : void
*
* PARAMETERS    : sd_session_t *
*
***********************************************************************/

	
void myfree(sdp_session_t *sess)
{
    /* Declaring and initializing variables for storing attribute addresses */
    sdp_list_t       *ptr_list = NULL;
    sdp_list_t	     *temp_list = NULL;
    sdp_conn_t       *ptr_conn = NULL;
    sdp_conn_t       *temp_conn = NULL;
    sdp_bandwidth_t  *ptr_bw = NULL;
    sdp_bandwidth_t  *temp_bw = NULL;
    sdp_zone_t       *ptr_zone = NULL;
    sdp_zone_t	     *temp_zone = NULL;
    sdp_attr_t       *ptr_attr = NULL;
    sdp_attr_t	     *temp_attr = NULL;
    sdp_media_t      *ptr_media = NULL;
    sdp_media_t      *temp_media = NULL;
    
    TRACE("\nTRACE: Entering function myfree\n");

    /***Freeing origin attribute ***/
    free(sess->s_origin->o_username);
    free(sess->s_origin->o_nettype);
    free(sess->s_origin->o_addrtype);
    free(sess->s_origin->o_address);
    
    free(sess->s_origin);
    
    /***Freeing name attribute ***/
    free(sess->s_name);
    
    /***Freeing information attribute ***/
    if(sess->s_info != NULL)
    {
        free(sess->s_info);
    }
    
    /***Freeing uri attribute ***/
    if(sess->s_uri != NULL)
    {
        free(sess->s_uri);
    }
    
    /***Freeing email attribute ***/
    ptr_list = sess->s_email;
    if(sess->s_email != NULL)
    while(TRUE == TRUE)
    {
        free(ptr_list->value);
        if(ptr_list->next == NULL)
	{
	    free(ptr_list);
	    break;
	}
	else
	{
	    temp_list = ptr_list;
	    ptr_list = ptr_list->next;
	    free(temp_list);
	}
    }
    
    /***Freeing phone attribute ***/
    ptr_list = sess->s_phone;
    if(sess->s_phone != NULL)
    while(TRUE == TRUE)
    {
        free(ptr_list->value);
        if(ptr_list->next == NULL)
	{  
	    free(ptr_list);
	    break;
	}
	else
	{
	    temp_list = ptr_list;
	    ptr_list = ptr_list->next;
	    free(temp_list);
	}
    }

    
    /***Freeing Connection attribute***/
    ptr_conn = sess->s_conn;
    if(sess->s_conn != NULL)
    while(TRUE == TRUE)
    {
        free(ptr_conn->c_nettype);
        free(ptr_conn->c_addrtype);
        free(ptr_conn->c_address);
        if(ptr_conn->c_next == NULL)
	{
	    free(ptr_conn);
	    break;
	}
	else
	{
	    temp_conn = ptr_conn;
	    ptr_conn = ptr_conn->c_next;
	    free(temp_conn);
	}
    }
    
    
    /***Freeing bandwidth attribute ***/
    ptr_bw = sess->s_bw;
    if(ptr_bw != NULL)
    while(TRUE == TRUE)
    {
        free(ptr_bw->b_type);
        if(ptr_bw->b_next == NULL)
	{
	    free(ptr_bw);
	    break;
	}
	else
	{
	    temp_bw = ptr_bw;
	    ptr_bw  = ptr_bw->b_next;
	    free(temp_bw);
	}
    }
    
    /***Freeing time attribute ***/
    if(sess->s_time->t_repeat != NULL)
    {
    	ptr_list=sess->s_time->t_repeat->r_offset;
    	while(TRUE == TRUE)
    	{
            free(ptr_list->value);
            if(ptr_list->next == NULL)
	    {
	        free(ptr_list);
	    	break;
	    }
	    else
	    {
	    	temp_list=ptr_list;
		ptr_list = ptr_list->next;
	    	free(temp_list);
	    }
    	}
    	free(sess->s_time->t_repeat);
    	free(sess->s_time);
    }
    
    /***Freeing zone attribute***/
    ptr_zone = sess->s_zone;
    if(ptr_zone != NULL)
    {
        while(TRUE == TRUE)
        {
            free(ptr_zone->z_offset);
            if(ptr_zone->z_next == NULL)
	    {   
	        free(ptr_zone);
		break;
	    }
	    else
	    {
		temp_zone=ptr_zone;
		ptr_zone = ptr_zone->z_next;
		free(temp_zone);
	    }
        }
    }

    /***Freeing key attribute ***/
    if(sess->s_key != NULL)
    {
        free(sess->s_key->k_method);
        free(sess->s_key);
    }
    
    /***Freeing attribute ***/
    if(sess->s_attr != NULL)
    {
        free(sess->s_attr->a_name);
	free(sess->s_attr);
    }

    /***Freeing media attributes***/
    ptr_media = sess->s_media;
    if(ptr_media != NULL)
    	while(TRUE == TRUE)
    	{
            free(ptr_media->m_name);
            free(ptr_media->m_proto);
	    ptr_list = ptr_media->m_format;
	    while(TRUE == TRUE)
	    {
	    	free(ptr_list->value);
            	if(ptr_list->next == NULL)
	    	{   
	            free(ptr_list);
		    break;
            	}
		else
		{
	            temp_list=ptr_list;
	            ptr_list = ptr_list->next;
	     	    free(temp_list);
	    	}
	    }
    	    if(ptr_media->m_info != NULL)
    	    {
            	free(ptr_media->m_info);
    	    }

	    if(ptr_media->m_conn != NULL)
	    {
                while(TRUE == TRUE)
        	{	
            	    free(ptr_media->m_conn->c_nettype);
            	    free(ptr_media->m_conn->c_addrtype);
            	    free(ptr_media->m_conn->c_address);
            	    if(ptr_media->m_conn->c_next == NULL)
	            {
	        	free(ptr_media->m_conn);
	        	break;
	    	    }
	    	    else
	    	    {
	                temp_conn = ptr_media->m_conn;
	        	ptr_media->m_conn = ptr_media->m_conn->c_next;
	        	free(temp_conn);
	    	    }
        	}
	    }


            if(ptr_media->m_bw != NULL)
            {
            	free(ptr_media->m_bw->b_type);
	    	free(ptr_media->m_bw);
            }

            if(ptr_media->m_key != NULL)
            {
            	free(ptr_media->m_key->k_method);
	    	free(ptr_media->m_key);
            }
            ptr_attr = ptr_media->m_attr;
        	
	    if(ptr_attr != NULL)
            while(TRUE == TRUE)
            {
           	free(ptr_attr->a_name);
           	free(ptr_attr->a_value);
           	if(ptr_attr->a_next == NULL)
	    	{   
	            free(ptr_attr);
	            break;
	    	}
	    	else
	    	{
	            temp_attr=ptr_attr;
	            ptr_attr = ptr_attr->a_next;
	            free(temp_attr);
	    	}
            }
	
            if(ptr_media->m_next == NULL)
	    {
	        free(ptr_media);
	    	break;
	    }
	    else
	    {
	        temp_media=ptr_media;
	    	ptr_media= ptr_media->m_next;
	        free(temp_media);
	    }
    	}
    free(sess);
    TRACE("\nTRACE: Exiting function myfree with Success\n");

    return;
}
