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
	Author:		Almamu, Aknor Jaden, adapted from /src/eve-server/system/BubbleManager.cpp authored by Zhur
*/

#include "EVEServerPCH.h"

/*
 * std::map<uint32, ItemGroup *>::iterator res = m_groups.find(contractID);
 */

ContractFactory::ContractFactory(
	ItemFactory &_factory)
	: m_itemFactory(_factory)
{
	std::map<uint32, ContractRef> contracts;
	db().LoadContracts( contracts, m_itemFactory, *this );
	m_contracts = contracts;
}

ContractFactory::~ContractFactory()
{
	std::map<uint32, ContractRef>::const_iterator cur, end;

	cur = m_contracts.begin();
	end = m_contracts.end();

	for(; cur != end; cur++)
	{
		delete &cur->second;
	}

	m_contracts.clear();
}

template<class _Ty>
RefPtr<_Ty> ContractFactory::_GetContract( uint32 contractID )
{
	std::map<uint32, ContractRef>::iterator res = m_contracts.find( contractID );

	if( res == m_contracts.end() )
	{
		// Load the item
		RefPtr<_Ty> contract = _Ty::Load( m_itemFactory, *this, contractID );
		if( !contract ) return RefPtr<_Ty>();

		// We keep the original ref.
		res = m_contracts.insert( std::make_pair( contractID, contract ) ).first;
	}

	return RefPtr<_Ty>::StaticCast( res->second );
}

ContractRef ContractFactory::GetContract( uint32 contractID )
{
	return _GetContract<Contract>( contractID );
}


void ContractFactory::DeleteContract( uint32 contractID )
{
	std::map<uint32, ContractRef>::iterator res = m_contracts.find( contractID );

	if( res == m_contracts.end() ) sLog.Error( "Contract Factory", "Contract ID %u not found when requesting deletion!", contractID );
	else m_contracts.erase( res );
}

uint32 ContractFactory::CreateContract( ContractRef contractInfo )
{
	uint32 contractID = db().CreateContract( contractInfo );

	ContractRef contract  = ContractRef( new Contract( contractID, (ContractData &)contractInfo->contractData(), contractInfo->requestItems(), contractInfo->items(), contractInfo->itemFactory(), contractInfo->contractFactory()  ) );
	m_contracts.insert( std::make_pair( contractID, contract) ).first;

	return contractID;
}


void ContractFactory::AddContract( ContractRef contract )
{
	uint32 contractID = contract->contractID();
	m_contracts.insert( std::make_pair( contractID, contract ) ).first;
}


