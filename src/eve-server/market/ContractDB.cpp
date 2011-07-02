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


bool ContractDB::LoadContracts( std::vector<Contract*> &into )
{
	std::vector<Contract*> contracts;
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
		return false;
	}

	uint32 i = 0;
	uint32 n = 0;
	while( res.GetRow( row ) )
	{

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

		// Assign contract a pointer to cData
		contracts.at( i )->m_contract = *cData;

		if( !sDatabase.RunQuery(items,
			"SELECT"
			" typeID,"
			" quantity"
			" FROM contract_items"
			" WHERE contractID=%u AND get=1", row.GetUInt( 0 ) ))
		{
			_log(DATABASE__ERROR, "Error loading items for contract %u", row.GetUInt( 0 ) );
			return false; // Return the actual load state
		}
			
		while( items.GetRow( item ) )
		{
			contracts.at( row.GetUInt( 0 ) )->m_requestItemTypeList.insert( contracts.at( row.GetUInt( 0 ) )->m_requestItemTypeList.begin(), contracts.at( row.GetUInt( 0 ) )->m_requestItemTypeList.end() );
			n = contracts.at( row.GetUInt( 0 ) )->m_requestItemTypeList.size();
			contracts.at( row.GetUInt( 0 ) )->m_requestItemTypeList.at( n ).m_typeID = item.GetUInt( 0 );
			contracts.at( row.GetUInt( 0 ) )->m_requestItemTypeList.at( n ).m_quantity = item.GetUInt( 1 );
		}
	}

	return true;
}

bool ContractDB::GetContractItems( uint32 contractID, std::vector<uint32>& into )
{
	DBQueryResult res;
	DBResultRow row;

	if( !sDatabase.RunQuery(res,
		"SELECT"
		" itemID"
		" FROM contract_items"
		" WHERE contractID=%u AND get=0", contractID ))
	{
		_log(DATABASE__ERROR, "Error loading items for contract %d", contractID );
		return false; // Return the actual load state...
	}

	while( res.GetRow( row ) )
	{
		into.push_back( row.GetUInt( 0 ) );
	}

	return true;
}

bool ContractDB::SaveContract( Contract* contract )
{
	DBerror err;

	if(!sDatabase.RunQueryLID(err, contract->m_contract.m_contractID,
		"INSERT INTO"
		" contract("
		" contractID,"
		" issuerID,"
		" issuerCorpID,"
		" type,"
		" avail,"
		" assigneeID,"
		" expiretime,"
		" duration,"
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
		" volume," // This should be the volume of all the items
		" issuerWalletKey,"
		" issuerAllianceID,"
		" crateID"
		")VALUES("
		"NULL, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %f, %f, %f, '%s', '%s', %u, 0, false, NULL, " I64u ", " I64u ", NULL, " I64u ", %f, %d, %d, %u)",
		contract->m_contract.m_issuerID, contract->m_contract.m_issuerCorpID, contract->m_contract.m_type, contract->m_contract.m_avail,
		contract->m_contract.m_assigneeID, contract->m_contract.m_expiretime, contract->m_contract.m_duration, contract->m_contract.m_startStationID,
		contract->m_contract.m_endStationID, contract->m_contract.m_startSolarSystemID, contract->m_contract.m_endSolarSystemID,
		contract->m_contract.m_startRegionID, contract->m_contract.m_endRegionID, contract->m_contract.m_price, contract->m_contract.m_reward,
		contract->m_contract.m_collateral, contract->m_contract.m_title, contract->m_contract.m_description,
		contract->m_contract.m_forCorp, contract->m_contract.m_dateIssued, contract->m_contract.m_dateExpired,
		contract->m_contract.m_dateExpired,	contract->m_contract.m_volume, contract->m_contract.m_issuerWalletKey,
		contract->m_contract.m_allianceID, contract->m_contract.m_crateID))
	{
		codelog(DATABASE__ERROR, "Error in query: %s", err.c_str() );
		return false;
	}

	std::string query;
	uint32 itemID = 0;
	uint32 contractID = contract->m_contract.m_contractID;
	uint32 quantity = 0;
	uint32 flag = 0;
	uint32 typeID = 0;
	uint32 ownerID = contract->m_contract.m_issuerID;

	for( size_t i = 0; i < contract->m_itemList.size(); i ++)
	{
		itemID = contract->m_itemList.at( i )->itemID();
		quantity = contract->m_itemList.at( i )->quantity();
		flag = contract->m_itemList.at( i )->flag();
		typeID = contract->m_itemList.at( i )->typeID();

		char buf[ 128 ];
		snprintf( buf, 128, "(NULL, %u, %u, %u, %u, %u, %u, false, false)", itemID, contractID, quantity, flag, typeID, ownerID );

		if( i != 0 )
            query += ',';
        query += buf;

		contract->m_itemList.at( i )->Move( contract->m_contract.m_contractID, flagBriefcase, true );
		contract->m_itemList.at( i )->ChangeOwner( 1, true );
	}

	if( !sDatabase.RunQuery( err,
		"INSERT"
		" INTO contract_items(id, itemID, contractID, quantity, flag, typeID, ownerID, inCrate, get)"
		" VALUES %s", query.c_str() ))
	{
		_log(DATABASE__ERROR, "Failed to insert Contract items for contract %u", contractID );
		return false;
	}

	// Request items
	for( size_t i = 0; i < contract->m_requestItemTypeList.size(); i ++)
	{
		quantity = contract->m_requestItemTypeList.at( i ).m_quantity;
		typeID = contract->m_requestItemTypeList.at( i ).m_typeID;

		char buf[ 128 ];
		snprintf( buf, 128, "(NULL, NULL, %u, %u, NULL, %u, NULL, false, false)", contractID, quantity, typeID );

		if( i != 0 )
            query += ',';
        query += buf;
	}

	if( !sDatabase.RunQuery( err,
		"INSERT"
		" INTO contract_items(id, itemID, contractID, quantity, flag, typeID, ownerID, inCrate, get)"
		" VALUES %s", query.c_str() ))
	{
		_log(DATABASE__ERROR, "Failed to insert Contract items for contract %u", contractID );
		return false;
	}
	return true;
}


Contract* GetContractInfo( uint32 contractID )
{
	Contract* contract;
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
		" FROM contract"
		" WHERE contractID=%u", contractID ))
	{
		_log(DATABASE__ERROR, "Error loading contracts. Error: %s", res.error.c_str() );
		return contract;
	}

	if( !res.GetRow( row ) )
	{
		_log(DATABASE__ERROR, "Cant fetch contract %u. Error: %s", contractID, res.error.c_str() );
	}

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
	contract->m_contract = *cData;

	if( !sDatabase.RunQuery(items,
		"SELECT"
		" typeID,"
		" quantity"
		" FROM contract_items"
		" WHERE contractID=%u AND get=1", row.GetUInt( 0 ) ))
	{
		_log(DATABASE__ERROR, "Error loading items for contract %u", row.GetUInt( 0 ) );
		return contract; // Return the actual load state
	}
	
	uint32 n = 0;

	while( items.GetRow( item ) )
	{
		contract->m_requestItemTypeList.insert( contract->m_requestItemTypeList.begin(), contract->m_requestItemTypeList.end() );
		n = contract->m_requestItemTypeList.size();
		contract->m_requestItemTypeList.at( n ).m_typeID = item.GetUInt( 0 );
		contract->m_requestItemTypeList.at( n ).m_quantity = item.GetUInt( 1 );
	}

	return contract;
}


bool ContractDB::PrepareDBForContractsSave()
{
	DBerror err;

	if( !sDatabase.RunQuery(err,
		"DELETE FROM contract" ))
	{
		_log(DATABASE__ERROR, "Error deleting contracts for final contract saving" );
		return false;
	}

	return true;
}






