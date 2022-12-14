/***************************************************************************\
 * auks_acl.h - AUKS ACL functions and structures definitions
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
#ifndef __AUKS_ACL_H_
#define __AUKS_ACL_H_

/*! \addtogroup AUKS_ACL
 *  @{
 */

enum AUKS_ACL_ROLE {
	AUKS_ACL_ROLE_UNKNOWN = 0,
	AUKS_ACL_ROLE_GUEST,
	AUKS_ACL_ROLE_USER,
	AUKS_ACL_ROLE_ADMIN
};

typedef struct auks_acl_rule {
	char *principal;
	char *host;
	int role;
} auks_acl_rule_t;


typedef struct auks_acl {
	auks_acl_rule_t *rules_array;
	int rules_nb;
	int rules_nb_max;
} auks_acl_t;


/*!
 * \brief Initialise an auks acl structure
 *
 * \param p_acl pointer to the structure to initialise
 * \param max_rules_nb max number of rules that could 
 * be managed by this acl structure
 *
 * \retval AUKS_SUCCESS operation successfully done
 * \retval AUKS_ERROR operation failed
*/
int auks_acl_init(auks_acl_t * p_acl, int max_rules_nb);

/*!
 * \brief Initialise an auks acl structure using an acl file
 *
 * \param p_acl pointer to the structure to initialise
 * \param acl_file file containing acl rules
 *
 * \retval AUKS_SUCCESS operation successfully done
 * \retval AUKS_ERROR operation failed
*/
int auks_acl_init_from_config_file(auks_acl_t * p_acl, char *acl_file);

/*!
 * \brief Free auks acl contents
 *
 * \param p_acl pointer to the structure to finalize
 *
 * \retval AUKS_SUCCESS operation successfully done
 * \retval AUKS_ERROR operation failed
*/
int auks_acl_free_contents(auks_acl_t * p_acl);

/*!
 * \brief Add rule to auks acl structure
 *
 * \param p_acl pointer to the structure to add rule to
 * \param principal regexp of matching principal of the rule
 * \param host hostname that match the rule
 * \param role associated role
 *
 * \retval AUKS_SUCCESS operation successfully done
 * \retval AUKS_ERROR operation failed
*/
int
auks_acl_add_rule(auks_acl_t * p_acl, char *principal, char *host,
		  enum AUKS_ACL_ROLE role);

/*!
 * \brief Get principal/host role 
 *
 * \param p_acl pointer to the structure to finalize
 * \param principal princiapl name of requester
 * \param host requester IP
 * \param role output role associated with this principal/host couple
 *
 * \retval AUKS_SUCCESS success
 * \retval AUKS_ERROR error
*/
int
auks_acl_get_role(auks_acl_t * p_acl, char *principal, char *host,
		  enum AUKS_ACL_ROLE *role);

/*!
 * @}
*/

#endif
