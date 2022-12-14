/***************************************************************************\
 * xmessage.h - xmessage functions and structures definitions
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
#ifndef __XMESSAGE_H_
#define __XMESSAGE_H_

/*! \addtogroup XTERNAL
 *  @{
 */

/*! \addtogroup XMESSAGE
 *  @{
 */

/*!
 * \enum XMESSAGE_TYPE
 */
enum XMESSAGE_TYPE {

  XPING_REQUEST=0,
  XGET_REQUEST,
  XEND_REQUEST,

  XERROR_REPLY=20,
  XPING_REPLY,
  XGET_REPLY,
  XACK_REPLY

};

/*!
 * \struct xmessage
 * \typedef xmessage_t
 * \brief external stream communication basic element
 */
typedef struct xmessage {
  int type;
  size_t length;
  void* data;
} xmessage_t;

/*!
 * \fn xmessage_init(xmessage_t* msg,int type,char* buffer,size_t length)
 * \brief initialise a xmessage structure
 *
 * \param msg pointer to the structure to initialise
 * \param type type of the message to initialize
 * \param buffer message body
 * \param length length of the message body
 *
 * \retval XSUCCESS operation successfully done
 * \retval XERROR generic error
 *
 */
int
xmessage_init(xmessage_t* msg,int type,char* buffer,size_t length);

/*!
 * \fn xmessage_marshall(xmessage_t* msg,char** pbuffer,size_t* psize)
 * \brief marshall an xmessage
 *
 * \param msg xmessage pointer
 * \param pbuffer pointer on a char buffer that will be allocated and store message data
 * \param psize newly allocated buffer size
 *
 * \retval XSUCCESS operation successfully done
 * \retval XERROR generic error
 *
 */
int
xmessage_marshall(xmessage_t* msg,char** pbuffer,size_t* psize);

/*!
 * \fn xmessage_unmarshall(xmessage_t* msg,char* buffer,size_t size)
 * \brief unmarshall a xmessage
 *
 * \param msg xmessage pointer
 * \param buffer marshalled buffer that represent the message
 * \param size marshalled buffer size
 *
 * \retval XSUCCESS operation successfully done
 * \retval XERROR generic error
 *
 */
int
xmessage_unmarshall(xmessage_t* msg,char* buffer,size_t size);

/*!
 * \fn xmessage_free_contents(xmessage_t* msg)
 * \brief free an xmessage structure contents
 *
 * \param msg pointer to the structure to free its contents
 *
 * \retval XSUCCESS operation successfully done
 * \retval XERROR generic error
 *
 */
int
xmessage_free_contents(xmessage_t* msg);

/*!
 * @}
*/

/*!
 * @}
*/

#endif
