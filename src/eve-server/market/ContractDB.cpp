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
	Author:		Almamu
*/


#include "EVEServerPCH.h"


std::map<uint32, Contract> ContractDB::LoadContracts()
{
	std::map<uint32, Contract> m_contracts;
	DBQueryResult res, items;
	DBResultRow row, item;

	if( !sDatabase.RunQuery( res,
		"SELECT"
		" contractID,"
		" issuerID,"
		" issuerCorpID,"
		" type,"
		" avail AS availability,"
		" assigneeID,"
		" expiretime,"
		" duration AS numDays,"
		" startStationID,"
		" endStationID,"
		" startSolarSystemID,"
		" endSolarSystemID,"
		" startRegionID,"
		" endRegionID,"
		" price,"
		" reward,"
		" collateral,"
		" title,"
		" description,"
		" forCorp,"
		" status,"
		" isAccepted,"
		" acceptorID,"
		" dateIssued,"
		" dateExpired,"
		" dateAccepted,"
		" dateCompleted,"
		" volume,"
		" issuerWalletKey,"
		" issuerAllianceID,"
		" acceptorWalletKey"
		" FROM contract" ))
	{
		_log(DATABASE__ERROR, "Error loading contracts. Error: %s", res.error.c_str() );
		return m_contracts;
	}

	uint32 i = 0;
	uint32 n = 0;
	while( res.GetRow( row ) )
	{
		// Insert new item
		m_contracts.insert( m_contracts.begin(), m_contracts.end() );

		// Get the index of the inserted item
		i = m_contracts.size();

		// Fill ContractData with the data from the DB
		ContractData* cData = new ContractData(
			row.GetUInt( 0 ),
			row.GetUInt( 1 ),
			row.GetUInt( 2 ),
			row.GetUInt( 3 ),
			row.GetUInt( 4 ),
			row.GetUInt( 5 ),
			row.GetUInt( 6 ),
			row.GetUInt( 7 ),
			row.GetUInt( 8 ),
			row.GetUInt( 9 ),
			row.GetUInt( 10 ),
			row.GetUInt( 11 ),
			row.GetUInt( 12 ),
			row.GetUInt( 13 ),
			row.GetDouble( 14 ),
			row.GetDouble( 15 ),
			row.GetDouble( 16 ),// 32
			row.GetText( 17 ),
			row.GetText( 18 ),
			row.GetUInt( 19 ), // bool
			row.GetUInt( 20 ),
			row.GetUInt( 21 ), // bool
			row.GetUInt( 22 ),
			row.GetUInt64( 23 ),
			row.GetUInt64( 24 ),
			row.GetUInt64( 25 ),
			row.GetUInt64( 26 ),
			row.GetDouble( 27 ),
			row.GetUInt( 28 ), // bool
			row.GetUInt( 29 ),
			row.GetUInt( 30 ),
			row.GetUInt( 0 )
			);

		// Assign m_contract a pointer to cData
		m_contracts.at( i ).m_contract = *cData;

		if( !sDatabase.RunQuery(items,
			"SELECT"
			" itemID"
			" FROM contract_items"
			" WHERE contractID=%u", row.GetUInt( 0 ) ))
		{
			_log(DATABASE__ERROR, "Error loading items for contract %d", row.GetUInt( 0 ) );
			return m_contracts; // Return the actual load state...
		}

		while( items.GetRow( item ) )
		{
			m_contracts.at( row.GetUInt( 0 ) ).m_itemList.insert( m_contracts.at( row.GetUInt( 0 ) ).m_itemList.begin(), m_contracts.at( row.GetUInt( 0 ) ).m_itemList.end() );
			n = m_contracts.at( row.GetUInt( 0 ) ).m_itemList.size();
			ItemFactory* factoryGive = m_contracts.at( row.GetUInt( 0 ) ).m_itemList.at( n ).GetItemFactory();
			m_contracts.at( row.GetUInt( 0 ) ).m_itemList.at( n ).Load( *factoryGive, item.GetUInt( 0 ) );
		}

		if( !sDatabase.RunQuery(items,
			"SELECT"
			" typeID,"
			" quantity"
			" FROM contract_items"
			" WHERE contractID=%u", row.GetUInt( 0 ) ))
		{
			_log(DATABASE__ERROR, "Error loading items for contract %u", row.GetUInt( 0 ) );
			return m_contracts; // Return the actual load state
		}
			
		while( items.GetRow( item ) )
		{
			m_contracts.at( row.GetUInt( 0 ) ).m_requestItemTypeList.insert( m_contracts.at( row.GetUInt( 0 ) ).m_requestItemTypeList.begin(), m_contracts.at( row.GetUInt( 0 ) ).m_requestItemTypeList.end() );
			n = m_contracts.at( row.GetUInt( 0 ) ).m_requestItemTypeList.size();
			m_contracts.at( row.GetUInt( 0 ) ).m_requestItemTypeList.at( n ).m_typeID = item.GetUInt( 0 );
			m_contracts.at( row.GetUInt( 0 ) ).m_requestItemTypeList.at( n ).m_quantity = item.GetUInt( 1 );
		}
	}

	return m_contracts;
}





