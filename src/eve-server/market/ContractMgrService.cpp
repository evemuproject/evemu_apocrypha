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

ContractMgrService::ContractMgrService(PyServiceMgr *mgr, ContractFactory* contractManager)
: PyService(mgr, "contractMgr"),
  m_dispatch(new Dispatcher(this)),
  m_contractManager(contractManager)
{
	_SetCallDispatcher(m_dispatch);

	PyCallable_REG_CALL(ContractMgrService, NumRequiringAttention);
	PyCallable_REG_CALL(ContractMgrService, CollectMyPageInfo);
	PyCallable_REG_CALL(ContractMgrService, GetItemsInStation);
	PyCallable_REG_CALL(ContractMgrService, GetContractListForOwner);
	PyCallable_REG_CALL(ContractMgrService, CreateContract);
	PyCallable_REG_CALL(ContractMgrService, GetContract);
	PyCallable_REG_CALL(ContractMgrService, GetContractList);
	PyCallable_REG_CALL(ContractMgrService, NumOutstandingContracts);
}

ContractMgrService::~ContractMgrService()
{
	delete m_dispatch;
}

PyResult ContractMgrService::Handle_NumOutstandingContracts( PyCallArgs& call )
{
	uint32 numOutstandingContracts = 0;
	std::map<uint32, ContractRef>::const_iterator cur, end;
	std::map<uint32, ContractRef> contracts = m_contractManager->GetContractList();

	cur = contracts.begin();
	end = contracts.end();

	for(; cur != end; cur++ )
	{
		ContractRef contract = cur->second;
		if( contract->issuerID() == call.client->GetCharacterID() )
		{
			numOutstandingContracts += 1;
		}
	}

	return new PyInt( numOutstandingContracts );
}

PyResult ContractMgrService::Handle_NumRequiringAttention( PyCallArgs& call )
{
	uint32 requiresAttentionCorp = 0;
	uint32 requiresAttention = 0;

	std::map<uint32, ContractRef>::const_iterator cur, end;
	std::map<uint32, ContractRef> contracts = m_contractManager->GetContractList();

	cur = contracts.begin();
	end = contracts.end();

	for(; cur != end; cur++ )
	{
		ContractRef contract = cur->second;
		if( contract->requiresAttention() )
		{
			if( contract->issuerCorpID() == call.client->GetCorporationID() )
			{
				requiresAttentionCorp += 1;
			}else if( contract->issuerID() == call.client->GetCharacterID() ){
				requiresAttention += 1;
			}else if( contract->acceptorID() == call.client->GetCharacterID() ){
				requiresAttention += 1;
			}else if( contract->assigneeID() == call.client->GetCharacterID() ){
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

	std::map<uint32, ContractRef>::const_iterator cur, end;
	std::map<uint32, ContractRef> contracts = m_contractManager->GetContractList();

	cur = contracts.begin();
	end = contracts.end();

	for(; cur != end; cur++ )
	{
		ContractRef contract = cur->second;
		if( contract->issuerID() == call.client->GetCharacterID() )
		{
			if( contract->forCorp() == true )
			{
				if( contract->requiresAttention() ) numRequiresAttentionCorp += 1;
				if( contract->assigneeID() ) numAssignedToCorp += 1;
				if( contract->status() == conStatusInProgress ) numInProgressCorp += 1;
				numOutstandingContractsForCorp += 1;
			}else{
				if( contract->requiresAttention() ) numRequiresAttention += 1;
				if( contract->assigneeID() ) numAssignedTo += 1;
				if( contract->status() == conStatusInProgress ) numInProgress += 1;
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
		new PyString( "util.KeyVal" ), ret 
		);
}


PyResult ContractMgrService::Handle_GetItemsInStation( PyCallArgs& call )
{
	Call_TwoIntegerArgs arg;
	uint32 stationID = 0;

	if( !arg.Decode( &call.tuple ) )
	{
		codelog(SERVICE__ERROR, "%s: Bad arguments to GetItemsInStation", call.client->GetName());
		stationID = call.client->GetStationID();
	}else stationID = arg.arg1;

	return m_db.GetPlayerItemsInStation( call.client->GetCharacterID(), stationID );
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
	
	std::map<uint32, ContractRef>::const_iterator cur, end;
	std::map<uint32, ContractRef> contracts = m_contractManager->GetContractList();

	cur = contracts.begin();
	end = contracts.end();

	util_Rowset res;

	for(; cur != end; cur++ )
	{
		if( cur->second->issuerID() != call.client->GetAccountID() ) continue;
		ContractRef contract = cur->second;
		PyPackedRow* into = rowset->NewRow();
		fieldData->AddItemInt( contract->contractID() );
		fieldData->AddItemInt( contract->issuerID() );
		fieldData->AddItemInt( contract->issuerCorpID() );
		fieldData->AddItemInt( contract->type() );
		fieldData->AddItemInt( contract->avail() );
		fieldData->AddItemInt( contract->assigneeID() );
		fieldData->AddItemInt( contract->expiretime() );
		fieldData->AddItemInt( contract->startStationID() );
		fieldData->AddItemInt( contract->endStationID() );
		fieldData->AddItemInt( contract->startSolarSystemID() );
		fieldData->AddItemInt( contract->endSolarSystemID() );
		fieldData->AddItemInt( contract->startRegionID() );
		fieldData->AddItemInt( contract->endRegionID() );
		fieldData->AddItemInt( contract->price() );
		fieldData->AddItemInt( contract->reward() );
		fieldData->AddItemInt( contract->collateral() );
		fieldData->AddItemString( contract->title().c_str() );
		fieldData->AddItemString( contract->description().c_str() );
		fieldData->AddItemInt( contract->forCorp() );
		fieldData->AddItemInt( contract->status() );
		fieldData->AddItemInt( contract->acceptorID() );
		fieldData->AddItemInt( contract->dateIssued() );
		fieldData->AddItemInt( contract->dateExpired() );
		fieldData->AddItemInt( contract->dateAccepted() );
		fieldData->AddItemInt( contract->dateCompleted() );
		fieldData->AddItemInt( contract->volume() );
		fieldData->AddItemInt( contract->issuerAllianceID() );
		fieldData->AddItemInt( contract->issuerWalletKey() );
		into->SetField( contract->contractID(), fieldData );
		fieldData = new PyList;

		res.lines = new PyList;
		res.header.clear();
		fieldData = new PyList;

		res.header.push_back( "itemTypeID" );
		res.header.push_back( "quantity" );
		res.header.push_back( "inCrate" );

		std::map<uint32, ContractRequestItemRef>::const_iterator rCur, rEnd;

		for(; rCur != rEnd; rCur++ )
		{
			fieldData->AddItemInt( rCur->second->m_typeID );
			fieldData->AddItemInt( rCur->second->m_quantity );
			fieldData->AddItemInt( false );
			res.lines->AddItem( fieldData );
			fieldData = new PyList;
		}

		std::map<uint32, ContractGetItemsRef>::const_iterator iCur, iEnd;

		for(; iCur != iEnd; iCur++ )
		{
			fieldData->AddItemInt( iCur->second->m_itemID );
			fieldData->AddItemInt( iCur->second->m_quantity );
			fieldData->AddItemInt( true );
			res.lines->AddItem( fieldData );
			fieldData = new PyList;
		}
	}

	_contract->SetItemString( "contracts",  rowset );
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
	
	std::map<uint32, ContractRef>::const_iterator cur, end;
	std::map<uint32, ContractRef> contracts = m_contractManager->GetContractList();

	cur = contracts.begin();
	end = contracts.end();

	util_Rowset res;

	for(; cur != end; cur++ )
	{
		ContractRef contract = cur->second;
		PyPackedRow* into = rowset->NewRow();
		fieldData->AddItemInt( contract->contractID() );
		fieldData->AddItemInt( contract->issuerID() );
		fieldData->AddItemInt( contract->issuerCorpID() );
		fieldData->AddItemInt( contract->type() );
		fieldData->AddItemInt( contract->avail() );
		fieldData->AddItemInt( contract->assigneeID() );
		fieldData->AddItemInt( contract->expiretime() );
		fieldData->AddItemInt( contract->duration() );
		fieldData->AddItemInt( contract->startStationID() );
		fieldData->AddItemInt( contract->endStationID() );
		fieldData->AddItemInt( contract->startSolarSystemID() );
		fieldData->AddItemInt( contract->endSolarSystemID() );
		fieldData->AddItemInt( contract->startRegionID() );
		fieldData->AddItemInt( contract->endRegionID() );
		fieldData->AddItemInt( contract->price() );
		fieldData->AddItemInt( contract->reward() );
		fieldData->AddItemInt( contract->collateral() );
		fieldData->AddItemString( contract->title().c_str() );
		fieldData->AddItemString( contract->description().c_str() );
		fieldData->AddItemInt( contract->forCorp() );
		fieldData->AddItemInt( contract->status() );
		fieldData->AddItemInt( contract->isAccepted() );
		fieldData->AddItemInt( contract->acceptorID() );
		fieldData->AddItemInt( contract->dateIssued() );
		fieldData->AddItemInt( contract->dateExpired() );
		fieldData->AddItemInt( contract->dateAccepted() );
		fieldData->AddItemInt( contract->dateCompleted() );
		fieldData->AddItemInt( contract->volume() );
		into->SetField( contract->contractID(), fieldData );
		fieldData = new PyList;

		res.lines = new PyList;
		res.header.clear();
		fieldData = new PyList;

		res.header.push_back( "itemTypeID" );
		res.header.push_back( "quantity" );
		res.header.push_back( "inCrate" );

		std::map<uint32, ContractRequestItemRef>::const_iterator rCur, rEnd;

		for(; rCur != rEnd; rCur++ )
		{
			fieldData->AddItemInt( rCur->second->m_typeID );
			fieldData->AddItemInt( rCur->second->m_quantity );
			fieldData->AddItemInt( false );
			res.lines->AddItem( fieldData );
			fieldData = new PyList;
		}

		std::map<uint32, ContractGetItemsRef>::const_iterator iCur, iEnd;

		for(; iCur != iEnd; iCur++ )
		{
			fieldData->AddItemInt( iCur->second->m_itemID );
			fieldData->AddItemInt( iCur->second->m_quantity );
			fieldData->AddItemInt( true );
			res.lines->AddItem( fieldData );
			fieldData = new PyList;
		}
	}

	_contract->SetItemString( "contracts",  rowset );
	_contract->SetItemString( "items", res.Encode() );
	_contract->SetItemString( "bids", new PyNone );

	return new PyObject(
		new PyString( "util.KeyVal" ), _contract
		);
}


PyResult ContractMgrService::Handle_CreateContract( PyCallArgs& call )
{
	Call_CreateContract info;
	PyList* requestItemTypeList = new PyList;
	PyList* itemList = new PyList;
	uint32 flag = 0;
	bool forCorp = false;
	double volume = 0;
	uint32 maxCharContracts = 0;

	if( call.byname.find( "forCorp" ) != call.byname.end() )
	{
		forCorp = call.byname.find( "forCorp" )->second->AsBool()->value();
	}

	// Let's see the players limit of contracts
	CharacterRef ch = call.client->GetChar();
	
	if( forCorp )
	{
		if( ch->HasSkill( 25233 ) )
		{
			SkillRef skill = ch->GetSkill( 25233 );
			uint32 skillLevel = skill->GetAttribute( 280 ).get_int();
			maxCharContracts = ( 10 * skillLevel ) + 10;
		}
		else
		{
			maxCharContracts = 10;
		}
	}
	else
	{
		if( ch->HasSkill( 25235 ) )
		{
			SkillRef skill = ch->GetSkill( 25235 );
			uint32 skillLevel = skill->GetAttribute( 280 ).get_int();
			maxCharContracts = ( 4 * skillLevel ) + 1;
		}
		else
		{
			maxCharContracts = 1;
		}

		uint32 numOutstandingContractsNonCorp = 0;
		uint32 numOutstandingContractsForCorp = 0;
		std::map<uint32, ContractRef>::const_iterator cur, end;
		std::map<uint32, ContractRef> contracts = m_contractManager->GetContractList();
		
		cur = contracts.begin();
		end = contracts.end();

		for(; cur != end; cur++ )
		{
			ContractRef contract = cur->second;
			if( contract->issuerID() == call.client->GetCharacterID() )
			{
				if( contract->forCorp() ) numOutstandingContractsForCorp += 1;
				else numOutstandingContractsNonCorp += 1;
			}
		}

		if( ( forCorp ) && ( numOutstandingContractsForCorp >= maxCharContracts ) )
		{
			call.client->SendInfoModalMsg( "Your Corporation Contracting skill level only allows you to create %d public contracts for your corp/alliance", maxCharContracts );
			return new PyBool( false );
		}

		if( ( !forCorp ) && ( numOutstandingContractsNonCorp >= maxCharContracts ) )
		{
			call.client->SendInfoModalMsg( "Your Contracting skill level only allows you to create %d public contracts", maxCharContracts );
			return new PyBool( false );
		}
	}

	if( !info.Decode( &call.tuple ) )
	{
		codelog(SERVICE__ERROR, "%s: Bad arguments to CreateContract in contractMgr", call.client->GetCharacterName() );
		return NULL;
	}

	if( call.byname.find( "requestItemTypeList" ) != call.byname.end() )
	{
		requestItemTypeList = call.byname.find( "requestItemTypeList" )->second->AsList();
	}

	if( call.byname.find( "flag" ) != call.byname.end() )
	{
		flag = call.byname.find( "flag" )->second->AsInt()->value();
	}

	if( call.byname.find( "itemList" ) != call.byname.end() )
	{
		itemList = call.byname.find( "itemList" )->second->AsList();
	}

	if( info.endStationID == 0 )info.endStationID = info.startStationID;

	ContractData* cData = new ContractData(
		call.client->GetCharacterID(),
		call.client->GetCorporationID(),
		info.type,
		info.avail,
		info.assigneeID,
		info.expiretime,
		info.expiretime,
		info.startStationID,
		info.endStationID,
		call.client->GetSystemID(),
		call.client->GetSystemID(),
		call.client->GetRegionID(),
		call.client->GetRegionID(),
		info.price,
		info.reward,
		info.collateral,
		info.title,
		info.description,
		forCorp,
		conStatusOutstanding,
		false,
		0,
		Win32TimeNow(),
		Win32TimeNow() + ( info.duration * Win32Time_Day ),
		Win32TimeNow(),
		Win32TimeNow(),
		0,
		false,
		0,
		0,
		0
		);

	std::map<uint32, ContractRequestItemRef> requestItems;
	std::map<uint32, ContractGetItemsRef> items;

	uint32 itemID = 0;
	uint32 typeID = 0;

	for( size_t i = 0; i < itemList->size(); i ++ )
	{
		if( itemList->IsList() )
		{
			if( itemList->GetItem( i )->AsList()->GetItem( 0 )->IsInt() )
				itemID = itemList->GetItem( i )->AsList()->GetItem( 0 )->AsInt()->value();
			else{
				sLog.Error( "ContractMgrService", "Wrong list args" );
				break;
			}
		}

		InventoryItemRef item = m_manager->item_factory.GetItem( itemID );

		if( item == NULL )
		{
			sLog.Error( "ContractMgrService", "GetItem returned NULL" );
			break;
		}

		item->Move( call.client->GetStationID(), flagBriefcase, true );
		item->ChangeOwner( 1, true );
		items.insert( std::make_pair( itemID, ContractGetItemsRef( new ContractGetItems( itemID,  itemList->GetItem( i )->AsList()->GetItem( 1 )->AsInt()->value() ) ) ) );
	}

	if( cData->m_type == conTypeItemExchange )
	{
		for( size_t i = 0; i < requestItemTypeList->size(); i ++ )
		{
			if( itemList->IsList() )
			{
				if( requestItemTypeList->GetItem( i )->AsList()->GetItem( 0 )->IsInt() )
					typeID = requestItemTypeList->GetItem( i )->AsList()->GetItem( 0 )->AsInt()->value();
				else{
					sLog.Error( "ContractMgrService", "Wrong list args" );
					break;
				}
			}

			requestItems.insert( std::make_pair( itemID, ContractRequestItemRef( new ContractRequestItem(itemID, requestItemTypeList->GetItem( i )->AsList()->GetItem( 1 )->AsInt()->value() ) ) ) );
		}
	}

	uint32 contractID = 0;
	sLog.Debug( "ContractMgrService", "Creating contract..." );
	ContractRef _contract = ContractRef( new Contract( contractID, *cData, requestItems, items, m_manager->item_factory, *m_contractManager ) );

	contractID = m_contractManager->CreateContract( _contract );
	sLog.Debug( "ContractMgrService", "Contract created" );
	
	return new PyInt( contractID );
}

PyResult ContractMgrService::Handle_GetContract( PyCallArgs& call )
{
	Call_SingleIntegerArg arg;
	PyDict* _contract = new PyDict;

	if( !arg.Decode( &call.tuple ) )
	{
		codelog(SERVICE__ERROR, "%s: Bad arguments to GetContract in contractMgr", call.client->GetCharacterName() );
		return NULL;
	}

	ContractRef contract = m_contractManager->GetContract( arg.arg );

	if( contract == NULL )
	{
		codelog(SERVICE__ERROR, "%s: Can't get contract %d, got NULL pointer!", call.client->GetCharacterName(), arg.arg );
		return NULL;
	}

	_contract->SetItemString( "items",		contract->GetItemsList() );
	_contract->SetItemString( "bids",		contract->GetBids() );
	_contract->SetItemString( "contract",	contract->GetContractRow() );

	PyObject* res = new PyObject( new PyString( "util.KeyVal" ), _contract );

	return res;
}










