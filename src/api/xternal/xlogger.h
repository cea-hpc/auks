/***************************************************************************\
 * xlogger.h - xlogger functions definitions
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
#ifndef __XLOGGER_H_
#define __XLOGGER_H_

#include <stdarg.h>

/*! \addtogroup XTERNAL
 *  @{
 */

/*! \addtogroup XLOGGER
 *  @{
 */

#define XVERBOSE_LEVEL_1   1
#define XVERBOSE_LEVEL_2   2
#define XVERBOSE_LEVEL_3   3

#define XDEBUG_LEVEL_1    1
#define XDEBUG_LEVEL_2    2
#define XDEBUG_LEVEL_3    3

/*!
 * \fn xerror_setmaxlevel(int level)
 * \brief set error max level that can be displayed
 *
 * \param level max level to set
 *
*/
void
xerror_setmaxlevel(int level);

/*!
 * \fn xerror_setstream(FILE* stream)
 * \brief set stream to use when printing error messages
 *
 * \param stream stream to use (default is stderr)
*/
void
xerror_setstream(FILE* stream);

/*!
 * \fn xerror(char* format,...)
 * \brief print error message of level 1
 *
 * \param format format to print optionnaly followed by args
*/
void
xerror(char* format,...);


/*!
 * \fn xverbose_setmaxlevel(int level)
 * \brief set verbose max level that can be displayed
 *
 * \param level max level to set
*/
void
xverbose_setmaxlevel(int level);

/*!
 * \fn xverbose_setstream(FILE* stream)
 * \brief set stream to use when printing verbose messages
 *
 * \param stream stream to use (default is stdout)
*/
void
xverbose_setstream(FILE* stream);

/*!
 * \fn xverbose(char* format,...)
 * \brief print verbose message of level 1
 *
 * \param format format to print optionnaly followed by args
*/
void
xverbose(char* format,...);

/*!
 * \fn xverbose2(char* format,...)
 * \brief print verbose message of level 2
 *
 * \param format format to print optionnaly followed by args
*/
void
xverbose2(char* format,...);

/*!
 * \fn xverbose3(char* format,...)
 * \brief print verbose message of level 3
 *
 * \param format format to print optionnaly followed by args
*/
void
xverbose3(char* format,...);

/*!
 * \fn xverboseN(int level,char* format,...)
 * \brief print verbose message of given level
 *
 * \param level level of the message (1<=N<=9)
 * \param format format to print optionnaly followed by args
*/
void
xverboseN(int level,char* format,...);


/*!
 * \fn xdebug_setmaxlevel(int level)
 * \brief set debug max level that can be displayed
 *
 * \param level max level to set
*/
void
xdebug_setmaxlevel(int level);

/*!
 * \fn xdebug_setstream(FILE* stream)
 * \brief set stream to use when printing debug messages
 *
 * \param stream stream to use (default is stdout)
*/
void
xdebug_setstream(FILE* stream);

/*!
 * \fn xdebug(char* format,...)
 * \brief print debug message of level 1
 *
 * \param format format to print optionnaly followed by args
*/
void
xdebug(char* format,...);

/*!
 * \fn xdebug2(char* format,...)
 * \brief print debug message of level 2
 *
 * \param format format to print optionnaly followed by args
*/
void
xdebug2(char* format,...);

/*!
 * \fn xdebug3(char* format,...)
 * \brief print debug message of level 3
 *
 * \param format format to print optionnaly followed by args
*/
void
xdebug3(char* format,...);

/*!
 * \fn xdebugN(int level,char* format,...)
 * \brief print debug message of given level
 *
 * \param level level of the message (1<=N<=9)
 * \param format format to print optionnaly followed by args
*/
void
xdebugN(int level,char* format,...);

/*!
 * @}
*/

/*!
 * @}
*/

#endif /* !__XLOGGER_H_ */
