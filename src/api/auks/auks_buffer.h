/***************************************************************************\
 * auks_buffer.h - auks_buffer functions and structures definitions
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
#ifndef __AUKS_BUFFER_H_
#define __AUKS_BUFFER_H_

/*! \addtogroup AUKS_BUFFER
 *  @{
 */

/* for XDR marshalling */
#include <rpc/types.h>
#include <rpc/xdr.h>

typedef struct auks_buffer {
	char *data;
	size_t length;
	size_t processed;
} auks_buffer_t;

/*!
 * \brief Initialise an empty auks buffer structure
 *
 * \param buf pointer to the structure to initialise
 * \param length minimal length of the buffer
 *
 * \retval AUKS_SUCCESS
 * \retval AUKS_ERROR
 * \retval AUKS_ERROR_BUFFER_MALLOC
 */
int
auks_buffer_init(auks_buffer_t * buf,size_t length);

/*!
 * \brief Initialize an auks buffer structure using a serialized one
 *
 * \param buf pointer to the structure to initialise
 * \param data serialized version of the auks buffer
 * \param length length of serialized data
 *
 * \retval AUKS_SUCCESS
 * \retval AUKS_ERROR
 */
int
auks_buffer_load(auks_buffer_t * buf,char * data,size_t length);


/*!
 * \brief Free an auks buffer structure contents
 *
 * \param buf pointer to the structure to free its contents
 *
 * \retval AUKS_SUCCESS operation successfully done
 * \retval AUKS_ERROR operation failed
 */
int
auks_buffer_free_contents(auks_buffer_t * buf);

/*!
 * \brief pack an integer into an auks buffer
 *
 * \param buf pointer to the structure to use
 * \param i integer value to pack
 *
 * \retval AUKS_SUCCESS
 * \retval AUKS_ERROR
 * \retval AUKS_ERROR_BUFFER_MALLOC
 * \retval AUKS_ERROR_BUFFER_REALLOC
 */
int
auks_buffer_pack_int(auks_buffer_t * buf,int i);

/*!
 * \brief unpack an integer from an auks buffer
 *
 * \param buf pointer to the structure to use
 * \param i pointer on an integer to unpack value to
 *
 * \retval AUKS_SUCCESS
 * \retval AUKS_ERROR
 */
int
auks_buffer_unpack_int(auks_buffer_t * buf,int * i);

/*!
 * \brief pack an uid value into an auks buffer
 *
 * \param buf pointer to the structure to use
 * \param u uid value to pack
 *
 * \retval AUKS_SUCCESS
 * \retval AUKS_ERROR
 * \retval AUKS_ERROR_BUFFER_MALLOC
 * \retval AUKS_ERROR_BUFFER_REALLOC
 */
int
auks_buffer_pack_uid(auks_buffer_t * buf,uid_t u);

/*!
 * \brief unpack an uid from an auks buffer
 *
 * \param buf pointer to the structure to use
 * \param u pointer on an uid_t to unpack value to
 *
 * \retval AUKS_SUCCESS
 * \retval AUKS_ERROR
 */
int
auks_buffer_unpack_uid(auks_buffer_t * buf,uid_t * u);

/*!
 * \brief pack data into an auks buffer
 *
 * \param buf pointer to the structure to use
 * \param data pointer on the array to pack
 * \param length length of the data to pack
 *
 * \retval AUKS_SUCCESS
 * \retval AUKS_ERROR
 * \retval AUKS_ERROR_BUFFER_MALLOC
 * \retval AUKS_ERROR_BUFFER_REALLOC
 */
int
auks_buffer_pack_data(auks_buffer_t * buf,char * data,size_t length);

/*!
 * \brief unpack data from an auks buffer
 *
 * \param buf pointer to the structure to use
 * \param data pointer on an array to unpack value to
 * \param length length of data to unpack
 *
 * \retval AUKS_SUCCESS
 * \retval AUKS_ERROR
 */
int
auks_buffer_unpack_data(auks_buffer_t * buf,char * data,size_t length);

/*!
 * @}
*/

#endif
