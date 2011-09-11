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


bool ContractDB::LoadContracts( std::map<uint32, ContractRef> &into, ItemFactory &item_factory, ContractFactory &contract_factory )
{
	DBQueryResult res, itemRes;
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
		" requiresAttention,"
		" issuerWalletKey,"
		" issuerAllianceID"
		" FROM contract" ))
	{
		_log(DATABASE__ERROR, "Error loading contracts. Error: %s", res.error.c_str() );
		return NULL;
	}

	uint32 i = 0;
	uint32 n = 0;
	while( res.GetRow( row ) )
	{
		uint32 contractID = row.GetUInt( 0 );

		// Fill ContractData with the data from the DB
		ContractData* cData = new ContractData(
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
			contractID
			);

		std::map<uint32, ContractRequestItemRef> requestItem;
		std::map<uint32, ContractGetItemsRef> items;

		if( !sDatabase.RunQuery(itemRes,
			"SELECT"
			" itemTypeID,"
			" quantity"
			" FROM contracts_items"
			" WHERE contractID=%u AND inCrate=0", contractID ))
		{
			_log(DATABASE__ERROR, "Error loading items for contract %u", row.GetUInt( 0 ) );
			return false; // Return the actual load state
		}
			
		while( itemRes.GetRow( item ) )
		{
			requestItem.insert( std::make_pair( item.GetUInt( 0 ), ContractRequestItemRef( new ContractRequestItem( item.GetUInt( 0 ), item.GetUInt( 1 ) ) ) ) ).first;
		}

		if( !sDatabase.RunQuery(itemRes,
			"SELECT"
			" itemID,"
			" quantity"
			" FROM contracts_items"
			" WHERE contractID=%u AND inCrate=1", contractID ))
		{
			_log(DATABASE__ERROR, "Error loading items for contract %u", row.GetUInt( 0 ) );
			return false; // Return the actual load state
		}
			
		while( itemRes.GetRow( item ) )
		{
			items.insert( std::make_pair( item.GetUInt( 0 ), ContractGetItemsRef( new ContractGetItems( item.GetUInt( 0 ), item.GetUInt( 1 ) ) ) ) ).first;

		}

		into.insert( std::make_pair( contractID, ContractRef( new Contract( contractID, *cData, requestItem, items, item_factory, contract_factory ) ) ) ).second;
	}

	return true;
}

bool ContractDB::GetContractItems( uint32 contractID, std::map<uint32, ContractGetItemsRef> &items, std::map<uint32, ContractRequestItemRef> &requestItems )
{
	DBQueryResult res;
	DBResultRow row;

	if( !sDatabase.RunQuery(res,
		"SELECT"
		" itemTypeID"
		" quantity"
		" FROM contracts_items"
		" WHERE contractID=%u AND inCrate = 0", contractID ))
	{
		_log(DATABASE__ERROR, "Error loading items for contract %d", contractID );
		return false; // Return the actual load state...
	}

	while( res.GetRow( row ) )
	{
		ContractRequestItemRef item;
		item->m_typeID = row.GetUInt( 0 );
		item->m_quantity = row.GetUInt( 1 );
		requestItems.insert( std::make_pair( item->m_typeID, item ) );
	}

	if( !sDatabase.RunQuery(res,
		"SELECT"
		" itemID,"
		" quantity"
		" FROM contracts_items"
		" WHERE contractID=%u AND inCrate = 1", contractID ))
	{
		_log(DATABASE__ERROR, "Error loading items for contract %d", contractID );
		return false;
	}

	while( res.GetRow( row ) )
	{
		ContractGetItemsRef item;
		item->m_itemID = row.GetUInt( 0 );
		item->m_quantity = row.GetUInt( 1 );
		items.insert( std::make_pair( item->m_itemID, item ) );
	}

	return true;
}

bool ContractDB::SaveContract( ContractRef contract )
{
	uint32 contractID = 0;
	DBerror err;

	sDatabase.RunQuery( err,"DELETE FROM contract WHERE contractID=%u", contract->contractID() );
	sDatabase.RunQuery( err, "DELETE FROM contracts_items WHERE contractID=%u", contract->contractID() );
	if(!sDatabase.RunQueryLID(err, contractID, 
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
		contract->issuerID(), contract->issuerCorpID(), contract->type(),
		contract->avail(), contract->assigneeID(), contract->expiretime(), contract->duration(),
		contract->startStationID(), contract->endStationID(), contract->startSolarSystemID(),
		contract->endSolarSystemID(), contract->startRegionID(), contract->endRegionID(),
		contract->price(), contract->reward(), contract->collateral(), contract->title().c_str(),
		contract->description().c_str(), contract->forCorp(), contract->dateIssued(),
		contract->dateExpired(), contract->dateExpired(), contract->volume(),
		contract->issuerWalletKey(), contract->issuerAllianceID(), 0 ))
	{
		codelog(DATABASE__ERROR, "Error in query: %s", err.c_str() );
		return false;
	}

	std::string query;
	uint32 itemID = 0;
	uint32 quantity = 0;
	uint32 typeID = 0;

	std::map<uint32, ContractRequestItemRef>::iterator cur, end;

	cur = contract->requestItems().begin();
	end = contract->requestItems().end();

	for(; cur != end; cur++ )
	{
		typeID = cur->second->m_typeID;
		quantity = cur->second->m_quantity;

		char buf[ 128 ];
		snprintf( buf, 128, "(NULL, %u, %u, %u, false, NULL)", contractID, typeID, quantity );

		if( cur != contract->requestItems().begin() )
			query += ',';
		query += buf;
	}
	if( !sDatabase.RunQuery( err,
		"INSERT"
		" INTO contracts_items(id, contractID, itemTypeID, quantity, inCrate, itemID)"
		" VALUES %s", query.c_str() ))
	{
		_log( DATABASE__ERROR, "Failed to insert Contract Request Items for contract %u", contractID );
		return false;
	}

	if( contract->type() == conTypeItemExchange )
	{
		std::map<uint32, ContractGetItemsRef>::iterator c, e;

		c = contract->items().begin();
		e = contract->items().end();
		query = ' ';

		for(; c != e; c++ )
		{
			itemID = c->second->m_itemID;
			quantity = c->second->m_quantity;

			char buf[ 128 ];
			snprintf( buf, 128, "(NULL, %u, %u, %u, true, %u)", contractID, typeID, quantity, itemID );

			if( c != contract->items().begin() )
				query += ',';
			query + buf;
		}

		if( !sDatabase.RunQuery( err,
			"INSERT INTO contracts_items"
			" (id, contractID, itemTypeID, quantity, inCrate, itemID)"
			" VALUES %s", query.c_str() ))
		{
			_log(DATABASE__ERROR, "Failed to insert Contract Items for contract %u", contractID );
			return false;
		}
	}

	return true;
}


ContractData *ContractDB::GetContractInfo( uint32 contractID )
{
	// Contract* contract;
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
		" requiresAttention,"
		" issuerWalletKey,"
		" issuerAllianceID"
		" FROM contract"
		" WHERE contractID=%u", contractID ))
	{
		_log(DATABASE__ERROR, "Error loading contracts. Error: %s", res.error.c_str() );
		return NULL;
	}

	if( !res.GetRow( row ) )
	{
		_log(DATABASE__ERROR, "Cant fetch contract %u. Error: %s", contractID, res.error.c_str() );
	}

	// Fill ContractData with the data from the DB
	ContractData* cData = new ContractData(
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
		contractID
		);
		
	return cData;

	/*
	if( !sDatabase.RunQuery( items,
		"SELECT itemTypeID,"
		" quantity"
		" FORM contract_items"
		" WHERE contractID = %u AND inCrate = false", row.GetUInt( 0 ) ))
	{
		_log(DATABASE__ERROR, "Error loading items for contract %u", row.GetUInt( 0 ) ));
		return NULL;
		}
	// Assign m_contract a pointer to cData
	/*contract->m_contract = *cData;

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

	return contract;*/
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


PyRep *ContractDB::GetPlayerItemsInStation( uint32 characterID, uint32 stationID )
{
	// Hack into NULL
	DBQueryResult res;

	if(!sDatabase.RunQuery(res,
		"SELECT itemID, itemName, typeID, ownerID, locationID, flag, contraband, singleton, quantity, x, y, z, custominfo, 0 AS categoryID, 0 AS groupID FROM entity WHERE  ownerID=%d && locationID=%d && flag=4", characterID, stationID)){
			codelog( MARKET__ERROR, "Error in query: %s", res.error.c_str() );
			return NULL;
	}
	return DBResultToRowset( res );
}


void ContractDB::DeleteContract( uint32 contractID )
{
	DBerror err;

	sDatabase.RunQuery( err, "DELETE FROM contract WHERE contractID=%u", contractID );
	sDatabase.RunQuery( err, "DELETE FROM contracts_items WHERE contractID=%u", contractID );

}


uint32 ContractDB::CreateContract( ContractRef contract )
{
	uint32 contractID = 0;
	DBerror err;

	if(!sDatabase.RunQueryLID(err, contractID,
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
		" issuerAllianceID"
		")VALUES("
		"NULL, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %f, %f, %f, ' ', ' ', %u, 0, false, 0, " I64u ", " I64u ", " I64u ", " I64u ", %f, %d, %d)",
		contract->issuerID(), contract->issuerCorpID(), contract->type(),
		contract->avail(), contract->assigneeID(), contract->expiretime(), contract->duration(),
		contract->startStationID(), contract->endStationID(), contract->startSolarSystemID(),
		contract->endSolarSystemID(), contract->startRegionID(), contract->endRegionID(),
		contract->price(), contract->reward(), contract->collateral(),
		contract->forCorp(), contract->dateIssued(), contract->dateExpired(),
		contract->dateAccepted(), contract->dateCompleted(), contract->volume(),
		contract->issuerWalletKey(), contract->issuerAllianceID() ))
	{
		codelog(DATABASE__ERROR, "Error in query: %s", err.c_str() );
		return false;
	}

	std::string query;
	uint32 itemID = 0;
	uint32 quantity = 0;
	uint32 typeID = 0;

	if( contract->type() == conTypeItemExchange )
	{
		std::map<uint32, ContractRequestItemRef>::iterator cur, end;
		std::map<uint32, ContractRequestItemRef> requestItems = contract->requestItems();

		cur = requestItems.begin();
		end = requestItems.end();

		for(; cur != end; cur++ )
		{
			typeID = cur->second->m_typeID;
			quantity = cur->second->m_quantity;

			char buf[ 128 ];
			snprintf( buf, 128, "(%u, %u, %u, false, 0)", contractID, typeID, quantity );

			if( ( cur != requestItems.begin() ) && ( cur != requestItems.end() ) )
				query += ',';
			query += buf;
		}

		query += ',';
	}

	std::map<uint32, ContractGetItemsRef>::iterator c, e;
	std::map<uint32, ContractGetItemsRef> items = contract->items();

	c = items.begin();
	e = items.end();

	typeID = 0;

	for(; c != e; c++ )
	{
		itemID = c->second->m_itemID;
		quantity = c->second->m_quantity;

		char buf[ 128 ];
		snprintf( buf, 128, "(%u, %u, %u, true, %u)", contractID, typeID, quantity, itemID );

		if( ( c != items.begin() ) && ( c != items.end() ) )
			query += ',';
		query += buf;
	}

	if( !sDatabase.RunQuery( err,
		"INSERT INTO contracts_items"
		" (contractID, itemTypeID, quantity, inCrate, itemID)"
		" VALUES %s", query.c_str() ))
	{
		_log(DATABASE__ERROR, "Failed to insert Contract Items for contract %u", contractID );
		return false;
	}

	return true;
}



