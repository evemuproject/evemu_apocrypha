/*
	------------------------------------------------------------------------------------
	LICENSE:
	------------------------------------------------------------------------------------
	This file is part of EVEmu: EVE Online Server Emulator
	Copyright 2006 - 2011 The EVEmu Team
	For the latest information visit http://evemu.org
	------------------------------------------------------------------------------------
	This program is free software; you can redistribute it and/or modify it under
	the terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2 of the License, or (at your option) any later
	version.

	This program is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
	FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this program; if not, write to the Free Software Foundation, Inc., 59 Temple
	Place - Suite 330, Boston, MA 02111-1307, USA, or go to
	http://www.gnu.org/copyleft/lesser.txt.
	------------------------------------------------------------------------------------
	Author:		Almamu, Aknor Jaden, adapted from /include/eve-server/system/BubbleManager.h authored by Zhur
*/

#include "EVEServerPCH.h"

#ifndef __CONTRACTFACTORY_H_INCL__
#define __CONTRACTFACTORY_H_INCL__


// The purpose of this object is to make a nice container for
// Contracts issued by Characters and Corporations.
// Its designed like ItemFactory
class ContractFactory {
public:
	ContractFactory(
		ItemFactory &_factory);
	~ContractFactory();

	ContractRef GetContract( uint32 contractID );
	uint32 CreateContract( ContractRef contractInfo );
	ContractDB& db() { return m_db; }
	void DeleteContract( uint32 contractID );
	void AddContract( ContractRef contract );
	std::map<uint32, ContractRef> GetContractList( ) const { return m_contracts; }

protected:
	template<class _Ty> RefPtr<_Ty> _GetContract( uint32 contractID );
	ContractDB m_db;

	ItemFactory &m_itemFactory;
	std::map<uint32, ContractRef> m_contracts;

};

#endif /* !__CONTRACTFACTORY__H__INCL__ */
