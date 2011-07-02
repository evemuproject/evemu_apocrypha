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


ContractManager::ContractManager()
{
	// ContractManager creation
}

void ContractManager::Load( ItemFactory item_factory )
{
	m_db.LoadContracts( m_contracts );
	std::map<uint32, uint32> items;
	uint32 j = 0;

	for( size_t i = 0; i < m_contracts.size(); i ++)
	{
		m_db.GetContractItems( m_contracts.at( i )->m_contract.m_contractID, items );
		for( size_t n = 0; n < items.size(); n ++ )
		{
			m_contracts.at( i )->m_itemList.insert( m_contracts.at( i )->m_itemList.begin(), m_contracts.at( i )->m_itemList.end() );
			j = m_contracts.at( i )->m_itemList.size();
			m_contracts.at( i )->m_itemList.at( j ) = item_factory.GetItem( items.at( n ) );
		}
	}
		
}

ContractManager::~ContractManager()
{
	// ContractManager destructor
	sLog.Debug( "ContractManager", "Saving contract information to the DB" );

	for( size_t i = 0; i < m_contracts.size(); i ++ )
	{
		m_db.SaveContract( m_contracts.at( i ) );
	}

	sLog.Debug( "ContractManager", "Destroying contract manager..." );
	clear();
	sLog.Debug( "ContractManager", "Contract manager destroyed" );
}

void ContractManager::clear()
{
	std::map<uint32, Contract*>::const_iterator cur, end;
	cur = m_contracts.begin();
	end = m_contracts.end();
	for(; cur != end; cur++)
    {
		delete &cur;
	}
	m_contracts.clear();
}

bool ContractManager::UpdateContract( Contract* contractInfo )
{
	uint32 i = 0;
	for( i = 0; i < m_contracts.size(); i ++ )
	{
		if( m_contracts.at( i )->m_contract.m_contractID == contractInfo->m_contract.m_contractID )
		{
			m_contracts.at( i ) = contractInfo;
			return true;
		}
	}
	return false;
}

void ContractManager::AddContract( Contract* contractInfo )
{
	uint32 i = 0;
	m_contracts.insert( m_contracts.begin(), m_contracts.end() );
	i = m_contracts.size();
	m_contracts.at( i ) = contractInfo;
}

bool ContractManager::GetContract( uint32 contractID, Contract* contract )
{
	uint32 i = 0;
	for( i = 0; i < m_contracts.size(); i ++ )
	{
		if( m_contracts.at( i )->m_contract.m_contractID == contractID )
		{
			contract = m_contracts.at( i );
			return true;
		}
	}

	return false;

}

bool ContractManager::RemoveContract( uint32 contractID )
{
	uint32 i = 0;
	for( i = 0; i < m_contracts.size(); i ++)
	{
		if( m_contracts.at( i )->m_contract.m_contractID == contractID )
		{
			m_contracts.erase( i );
			return true;
		}
	}

	return false;
}



