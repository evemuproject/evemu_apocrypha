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
	Author:		Zhur
*/

#include "EVEServerPCH.h"
//#include "ContractMgrService.h"



PyCallable_Make_InnerDispatcher(ContractMgrService)

ContractMgrService::ContractMgrService(PyServiceMgr *mgr, ContractManager* contractManager)
: PyService(mgr, "contractMgr"),
  m_dispatch(new Dispatcher(this)),
  m_contractManager(contractManager)
{
	_SetCallDispatcher(m_dispatch);

	PyCallable_REG_CALL(ContractMgrService, NumRequiringAttention);
	PyCallable_REG_CALL(ContractMgrService, CollectMyPageInfo);
	PyCallable_REG_CALL(ContractMgrService, GetItemsInStation);
	PyCallable_REG_CALL(ContractMgrService, GetContractListForOwner);
}

ContractMgrService::~ContractMgrService()
{
	delete m_dispatch;
}

PyResult ContractMgrService::Handle_NumRequiringAttention( PyCallArgs& call )
{
    sLog.Debug( "ContractMgrService", "Called NumRequiringAttention stub." );
	uint32 requiresAttentionCorp = 0;
	uint32 requiresAttention = 0;

	for( size_t i = 0; i < m_contractManager->m_contracts.size(); i ++ )
	{
		if( m_contractManager->m_contracts.at( i )->m_contract.m_requiresAttention )
		{
			if( m_contractManager->m_contracts.at( i )->m_contract.m_issuerCorpID == call.client->GetCorporationID() )
			{
				requiresAttentionCorp += 1;
			}else if( m_contractManager->m_contracts.at( i )->m_contract.m_issuerID == call.client->GetCharacterID() ){
				requiresAttention += 1;
			}else if( m_contractManager->m_contracts.at( i )->m_contract.m_acceptorID == call.client->GetCharacterID() ){
				requiresAttention += 1;
			}else if( m_contractManager->m_contracts.at( i )->m_contract.m_assigneeID == call.client->GetCharacterID() ){
				requiresAttention += 1;
			}
		}
	}
	PyDict* args = new PyDict;
	args->SetItemString( "n", new PyInt( requiresAttention ) );
	args->SetItemString( "ncorp", new PyInt( requiresAttentionCorp ) );

	return new PyObject(
        new PyString( "util.KeyVal" ), args
    );
}

PyResult ContractMgrService::Handle_CollectMyPageInfo( PyCallArgs& call )
{
	PyDict* ret = new PyDict;
	uint32 numOutstandingContractsNonCorp = 0;
	uint32 numOutstandingContractsForCorp = 0;
	uint32 numContractsLeft = 0;
	uint32 numRequiresAttention = 0;
	uint32 numRequiresAttentionCorp = 0;
	uint32 numAssignedTo = 0;
	uint32 numAssignedToCorp = 0;
	uint32 numBiddingOn = 0;
	uint32 numInProgress = 0;
	uint32 numBiddingOnCorp = 0;
	uint32 numInProgressCorp = 0;

	for( size_t i = 0; i < m_contractManager->m_contracts.size(); i ++ )
	{
		if( m_contractManager->m_contracts.at( i )->issuerID() == call.client->GetCharacterID() )
		{
			if( m_contractManager->m_contracts.at( i )->m_contract.m_forCorp == true )
			{
				if( m_contractManager->m_contracts.at( i )->m_contract.m_requiresAttention ) numRequiresAttentionCorp += 1;
				if( m_contractManager->m_contracts.at( i )->m_contract.m_assigneeID ) numAssignedToCorp += 1;
				if( m_contractManager->m_contracts.at( i )->m_contract.m_status == conStatusInProgress ) numInProgressCorp += 1;
				numOutstandingContractsForCorp += 1;
			}else{
				if( m_contractManager->m_contracts.at( i )->m_contract.m_requiresAttention ) numRequiresAttention += 1;
				if( m_contractManager->m_contracts.at( i )->m_contract.m_assigneeID ) numAssignedTo += 1;
				if( m_contractManager->m_contracts.at( i )->m_contract.m_status == conStatusInProgress ) numInProgress += 1;
				numOutstandingContractsNonCorp += 1;
			}

		}
	}
	ret->SetItemString( "numOutstandingContractsNonCorp", new PyInt( numOutstandingContractsNonCorp ) );
	ret->SetItemString( "numOutstandingContractsForCorp", new PyInt( numOutstandingContractsForCorp ) );
	ret->SetItemString( "numOutstandingContracts", new PyInt( numOutstandingContractsNonCorp + numOutstandingContractsForCorp ) );
	ret->SetItemString( "numContractsLeft", new PyInt( numContractsLeft ) );
	ret->SetItemString( "numRequiresAttention", new PyInt( numRequiresAttention ) );
	ret->SetItemString( "numRequiresAttentionCorp", new PyInt( numRequiresAttentionCorp ) );
	ret->SetItemString( "numAssignedTo", new PyInt( numAssignedTo ) );
	ret->SetItemString( "numAssignedToCorp", new PyInt( numAssignedToCorp ) );
	ret->SetItemString( "numBiddingOn", new PyInt( numBiddingOn ) );
	ret->SetItemString( "numInProgress", new PyInt( numInProgress ) );
	ret->SetItemString( "numBiddingOnCorp", new PyInt( numBiddingOnCorp ) );
	ret->SetItemString( "numInProgressCorp", new PyInt( numInProgressCorp ) );
	return new PyObject(
		new PyString( "util.Rowset"), ret 
		);
}


PyResult ContractMgrService::Handle_GetItemsInStation( PyCallArgs& call )
{
	return m_db.GetPlayerItemsInStation( call.client->GetCharacterID(), call.client->GetStationID() );
}


PyResult ContractMgrService::Handle_GetContractListForOwner( PyCallArgs& call )
{
	sLog.Debug( "ContractMgrService", "Called GetContractListForOwner stub." );
	Call_GetContractListForOwner arg;
	PyDict* _contract = new PyDict;

	if( !arg.Decode(&call.tuple) )
	{
		codelog(SERVICE__ERROR, "%s: Bad arguments to GetContractListForOwner", call.client->GetName());
		return NULL;
	}

	// Manual creation of a CRowset, i hate doing this -.-"
	DBRowDescriptor *header = new DBRowDescriptor();
	header->AddColumn( "contractID", DBTYPE_I4);
	header->AddColumn( "issuerID", DBTYPE_I4);
	header->AddColumn( "issuerCorpID", DBTYPE_I4 );
	header->AddColumn( "type", DBTYPE_UI1 );
	header->AddColumn( "availability", DBTYPE_I4 );
	header->AddColumn( "assigneeID", DBTYPE_I4 );
	header->AddColumn( "numDays", DBTYPE_I4 );
	header->AddColumn( "startStationID", DBTYPE_I4 );
	header->AddColumn( "endStationID", DBTYPE_I4 );
	header->AddColumn( "startSolarSystemID", DBTYPE_I4 );
	header->AddColumn( "endSolarSystemID", DBTYPE_I4 );
	header->AddColumn( "startRegionID", DBTYPE_I4 );
	header->AddColumn( "endRegionID", DBTYPE_I4 );
	header->AddColumn( "price", DBTYPE_CY );
	header->AddColumn( "reward", DBTYPE_CY );
	header->AddColumn( "collateral", DBTYPE_CY );
	header->AddColumn( "title", DBTYPE_WSTR );
	header->AddColumn( "description", DBTYPE_WSTR );
	header->AddColumn( "forCorp", DBTYPE_BOOL );
	header->AddColumn( "status", DBTYPE_UI1 );
	header->AddColumn( "acceptorID", DBTYPE_I4 );
	header->AddColumn( "dateIssued", DBTYPE_FILETIME );
	header->AddColumn( "dateExpired", DBTYPE_FILETIME );
	header->AddColumn( "dateAccepted", DBTYPE_FILETIME );
	header->AddColumn( "dateCompleted", DBTYPE_FILETIME );
	header->AddColumn( "volume", DBTYPE_R8 );
	header->AddColumn( "issuerAllianceID", DBTYPE_I4 );
	header->AddColumn( "issuerWalletKey", DBTYPE_I4 );
	CRowSet *rowset = new CRowSet( &header );

	PyList* fieldData = new PyList;
	DBResultRow row;
	
	for( size_t i = 0; i < m_contractManager->m_contracts.size( ); i ++ )
	{
		if( m_contractManager->m_contracts.at( i )->m_contract.m_issuerID == call.client->GetCharacterID() )
		{
			PyPackedRow* into = rowset->NewRow();
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_contractID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_issuerID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_issuerCorpID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_type );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_avail );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_assigneeID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_expiretime );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_duration );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_startStationID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_endStationID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_startSolarSystemID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_endSolarSystemID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_startRegionID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_endRegionID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_price );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_reward );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_collateral );
			fieldData->AddItemString( m_contractManager->m_contracts.at( i )->m_contract.m_title.c_str() );
			fieldData->AddItemString( m_contractManager->m_contracts.at( i )->m_contract.m_description.c_str() );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_forCorp );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_status );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_isAccepted );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_acceptorID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_dateIssued );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_dateExpired );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_dateAccepted );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_dateCompleted );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_volume );
			into->SetField( i, fieldData );
			fieldData = new PyList;
		}
	}

	_contract->SetItemString( "contracts",  rowset );

	util_Rowset res;
	res.lines = new PyList;
	res.header.clear();
	fieldData = new PyList;

	res.header.push_back( "itemTypeID" );
	res.header.push_back( "quantity" );
	res.header.push_back( "inCrate" );

	for( size_t i = 0; i < m_contractManager->m_contracts.size(); i ++ )
	{
		if( m_contractManager->m_contracts.at( i )->m_contract.m_issuerID == call.client->GetCharacterID() )
		{
			for( size_t n = 0; n < m_contractManager->m_contracts.at( i )->m_itemList.size(); n ++ )
			{
				fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_itemList.at( n )->typeID() );
				fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_itemList.at( n )->quantity() );
				fieldData->AddItemInt( true );
			}

			res.lines->AddItem( fieldData );
			fieldData = new PyList;

			for( size_t n = 0; n < m_contractManager->m_contracts.at( i )->m_requestItemTypeList.size(); n ++ )
			{
				fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_requestItemTypeList.at( n ).m_typeID );
				fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_requestItemTypeList.at( n ).m_quantity );
				fieldData->AddItemInt( false );
			}

			res.lines->AddItem( fieldData );
			fieldData = new PyList;
		}
	}

	_contract->SetItemString( "items", res.Encode() );

	_contract->SetItemString( "bids", new PyNone );
	return new PyObject(
		new PyString( "util.KeyVal" ), _contract
		);
}



PyResult ContractMgrService::Handle_GetContractList( PyCallArgs& call )
{
	sLog.Debug( "ContractMgrService", "Called GetContractListForOwner stub." );
	// Call_GetContractListForOwner arg;
	PyDict* _contract = new PyDict;

	/*if( !arg.Decode(&call.tuple) )
	{
		codelog(SERVICE__ERROR, "%s: Bad arguments to GetContractListForOwner", call.client->GetName());
		return NULL;
	}*/

	// Manual creation of a CRowset, i hate doing this -.-"
	DBRowDescriptor *header = new DBRowDescriptor();
	header->AddColumn( "contractID", DBTYPE_I4);
	header->AddColumn( "issuerID", DBTYPE_I4);
	header->AddColumn( "issuerCorpID", DBTYPE_I4 );
	header->AddColumn( "type", DBTYPE_UI1 );
	header->AddColumn( "availability", DBTYPE_I4 );
	header->AddColumn( "assigneeID", DBTYPE_I4 );
	header->AddColumn( "numDays", DBTYPE_I4 );
	header->AddColumn( "startStationID", DBTYPE_I4 );
	header->AddColumn( "endStationID", DBTYPE_I4 );
	header->AddColumn( "startSolarSystemID", DBTYPE_I4 );
	header->AddColumn( "endSolarSystemID", DBTYPE_I4 );
	header->AddColumn( "startRegionID", DBTYPE_I4 );
	header->AddColumn( "endRegionID", DBTYPE_I4 );
	header->AddColumn( "price", DBTYPE_CY );
	header->AddColumn( "reward", DBTYPE_CY );
	header->AddColumn( "collateral", DBTYPE_CY );
	header->AddColumn( "title", DBTYPE_WSTR );
	header->AddColumn( "description", DBTYPE_WSTR );
	header->AddColumn( "forCorp", DBTYPE_BOOL );
	header->AddColumn( "status", DBTYPE_UI1 );
	header->AddColumn( "acceptorID", DBTYPE_I4 );
	header->AddColumn( "dateIssued", DBTYPE_FILETIME );
	header->AddColumn( "dateExpired", DBTYPE_FILETIME );
	header->AddColumn( "dateAccepted", DBTYPE_FILETIME );
	header->AddColumn( "dateCompleted", DBTYPE_FILETIME );
	header->AddColumn( "volume", DBTYPE_R8 );
	header->AddColumn( "issuerAllianceID", DBTYPE_I4 );
	header->AddColumn( "issuerWalletKey", DBTYPE_I4 );
	CRowSet *rowset = new CRowSet( &header );

	PyList* fieldData = new PyList;
	DBResultRow row;
	
	for( size_t i = 0; i < m_contractManager->m_contracts.size( ); i ++ )
	{
		if( m_contractManager->m_contracts.at( i )->m_contract.m_startRegionID == call.client->GetRegionID () )
		{
			PyPackedRow* into = rowset->NewRow();
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_contractID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_issuerID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_issuerCorpID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_type );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_avail );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_assigneeID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_expiretime );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_duration );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_startStationID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_endStationID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_startSolarSystemID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_endSolarSystemID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_startRegionID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_endRegionID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_price );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_reward );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_collateral );
			fieldData->AddItemString( m_contractManager->m_contracts.at( i )->m_contract.m_title.c_str() );
			fieldData->AddItemString( m_contractManager->m_contracts.at( i )->m_contract.m_description.c_str() );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_forCorp );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_status );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_isAccepted );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_acceptorID );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_dateIssued );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_dateExpired );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_dateAccepted );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_dateCompleted );
			fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_contract.m_volume );
			into->SetField( i, fieldData );
			fieldData = new PyList;
		}
	}

	_contract->SetItemString( "contracts",  rowset );

	util_Rowset res;
	res.lines = new PyList;
	res.header.clear();
	fieldData = new PyList;

	res.header.push_back( "itemTypeID" );
	res.header.push_back( "quantity" );
	res.header.push_back( "inCrate" );

	for( size_t i = 0; i < m_contractManager->m_contracts.size(); i ++ )
	{
		if( m_contractManager->m_contracts.at( i )->m_contract.m_startRegionID == call.client->GetRegionID() )
		{
			for( size_t n = 0; n < m_contractManager->m_contracts.at( i )->m_itemList.size(); n ++ )
			{
				fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_itemList.at( n )->typeID() );
				fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_itemList.at( n )->quantity() );
				fieldData->AddItemInt( true );
			}

			res.lines->AddItem( fieldData );
			fieldData = new PyList;

			for( size_t n = 0; n < m_contractManager->m_contracts.at( i )->m_requestItemTypeList.size(); n ++ )
			{
				fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_requestItemTypeList.at( n ).m_typeID );
				fieldData->AddItemInt( m_contractManager->m_contracts.at( i )->m_requestItemTypeList.at( n ).m_quantity );
				fieldData->AddItemInt( false );
			}

			res.lines->AddItem( fieldData );
			fieldData = new PyList;
		}
	}

	_contract->SetItemString( "items", res.Encode() );

	_contract->SetItemString( "bids", new PyNone );
	return new PyObject(
		new PyString( "util.KeyVal" ), _contract
		);
}










