/***************************************************************************\
 * auks_message.c - AUKS communication messages implementation
 * based on auks_buffer
 ***************************************************************************
 * Copyright  CEA/DAM/DIF (2009)
 *
 * Written by Matthieu Hautreux <matthieu.hautreux@cea.fr>
 * 
 * This software is a computer program whose purpose is to simplify
 * the addition of kerberos credential support in Batch applications.
 *
 * This software is governed by the CeCILL-C license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL-C
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info". 
 * 
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability. 
 * 
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or 
 * data to be ensured and,  more generally, to use and operate it in the 
 * same conditions as regards security. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-C license and that you accept its terms.
 ***************************************************************************
 * Copyright  CEA/DAM/DIF (2009)
 * 
 * Ecrit par Matthieu Hautreux <matthieu.hautreux@cea.fr>
 *
 * Ce logiciel est un programme informatique servant ?? faciliter l'ajout
 * du support des tickets Kerberos aux applications Batch.
 * 
 * Ce logiciel est r??gi par la licence CeCILL-C soumise au droit fran??ais et
 * respectant les principes de diffusion des logiciels libres. Vous pouvez
 * utiliser, modifier et/ou redistribuer ce programme sous les conditions
 * de la licence CeCILL-C telle que diffus??e par le CEA, le CNRS et l'INRIA 
 * sur le site "http://www.cecill.info".
 * 
 * En contrepartie de l'accessibilit?? au code source et des droits de copie,
 * de modification et de redistribution accord??s par cette licence, il n'est
 * offert aux utilisateurs qu'une garantie limit??e.  Pour les m??mes raisons,
 * seule une responsabilit?? restreinte p??se sur l'auteur du programme,  le
 * titulaire des droits patrimoniaux et les conc??dants successifs.
 * 
 * A cet ??gard  l'attention de l'utilisateur est attir??e sur les risques
 * associ??s au chargement,  ?? l'utilisation,  ?? la modification et/ou au
 * d??veloppement et ?? la reproduction du logiciel par l'utilisateur ??tant 
 * donn?? sa sp??cificit?? de logiciel libre, qui peut le rendre complexe ?? 
 * manipuler et qui le r??serve donc ?? des d??veloppeurs et des professionnels
 * avertis poss??dant  des  connaissances  informatiques approfondies.  Les
 * utilisateurs sont donc invit??s ?? charger  et  tester  l'ad??quation  du
 * logiciel ?? leurs besoins dans des conditions permettant d'assurer la
 * s??curit?? de leurs syst??mes et ou de leurs donn??es et, plus g??n??ralement, 
 * ?? l'utiliser et l'exploiter dans les m??mes conditions de s??curit??. 
 * 
 * Le fait que vous puissiez acc??der ?? cet en-t??te signifie que vous avez 
 * pris connaissance de la licence CeCILL-C, et que vous en avez accept?? les
 * termes. 
\***************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#define AUKS_LOG_HEADER "auks_message: "
#define AUKS_LOG_BASE_LEVEL 4
#define AUKS_LOG_DEBUG_LEVEL 4

#include "auks/auks_error.h"
#include "auks/auks_buffer.h"
#include "auks/auks_cred.h"
#include "auks/auks_message.h"
#include "auks/auks_log.h"

int
auks_message_init(auks_message_t * msg,int type,
		  char * buffer,size_t length)
{
	int fstatus;

	msg->type = type;

	fstatus = auks_buffer_init(&msg->buffer,sizeof(int)+length);
	if ( fstatus != AUKS_SUCCESS )
		goto exit;

	fstatus = auks_buffer_pack_int(&msg->buffer,type);
	if ( fstatus != AUKS_SUCCESS )
		goto buf_exit;

	if ( buffer != NULL && length != 0 ) {
		fstatus = auks_buffer_pack_int(&msg->buffer,(int)length);
		if ( fstatus != AUKS_SUCCESS )
			goto buf_exit;
		fstatus = auks_buffer_pack_data(&msg->buffer,buffer,length);
	}

buf_exit:
	if ( fstatus != AUKS_SUCCESS )
		auks_buffer_free_contents(&msg->buffer);

exit:
	return fstatus;
}

int
auks_message_free_contents(auks_message_t * msg)
{
	int fstatus;

	msg->type = AUKS_PING_REQUEST;

	fstatus = auks_buffer_free_contents(&msg->buffer);

	return fstatus;
}

int
auks_message_marshall(auks_message_t * msg,char ** pbuffer,
		      size_t * psize)
{
	int fstatus = AUKS_ERROR ;
	
	char *buffer;
	size_t size;

	size = msg->buffer.processed ;
	buffer = (char *) malloc(size * sizeof(char));
	if (buffer == NULL) {
		auks_error("unable to allocate memory for "
			   "message marshalling");
		fstatus = AUKS_ERROR_MESSAGE_MALLOC ;
		goto exit;
	}
	memcpy(buffer,msg->buffer.data,size);

	*pbuffer = buffer;
	*psize = size;
	fstatus = AUKS_SUCCESS ;
	
exit:
	return fstatus;
}

int
auks_message_load(auks_message_t * msg,char *buffer,
		  size_t size)
{
	int fstatus;
	
	fstatus = auks_buffer_load(&msg->buffer,buffer,size);
	if ( fstatus != AUKS_SUCCESS )
		goto exit;
	
	fstatus = auks_buffer_unpack_int(&msg->buffer,&msg->type);
	
	if ( fstatus != AUKS_SUCCESS )
		auks_buffer_free_contents(&msg->buffer);
exit:
	return fstatus;
}


size_t auks_message_packed(auks_message_t * msg) {
	size_t packed;
	packed = msg->buffer.processed;
	return packed;
}

size_t auks_message_unpacked(auks_message_t * msg) {
	size_t packed;
	packed = msg->buffer.length - msg->buffer.processed;
	return packed;
}

char * auks_message_data(auks_message_t * msg) {
	return msg->buffer.data;
}


int auks_message_pack_int(auks_message_t * msg,int i)
{
	int fstatus;

	fstatus = auks_buffer_pack_int(&msg->buffer,i);

	return fstatus;
}

int auks_message_unpack_int(auks_message_t * msg,int * i)
{
	int fstatus;

	fstatus = auks_buffer_unpack_int(&msg->buffer,i);

	return fstatus;
}

int auks_message_pack_uid(auks_message_t * msg,uid_t u)
{
	int fstatus;

	fstatus = auks_buffer_pack_uid(&msg->buffer,u);

	return fstatus;
}

int auks_message_unpack_uid(auks_message_t * msg,uid_t * u)
{
	int fstatus;

	fstatus = auks_buffer_unpack_uid(&msg->buffer,u);

	return fstatus;
}

int auks_message_pack_data(auks_message_t * msg,char* data,size_t length)
{
	int fstatus;
	
	fstatus = auks_buffer_pack_data(&msg->buffer,data,length);
	
	return fstatus;
}

int auks_message_unpack_data(auks_message_t * msg,char* data,size_t length)
{
	int fstatus;
	
	fstatus = auks_buffer_unpack_data(&msg->buffer,data,length);
	
	return fstatus;
}
