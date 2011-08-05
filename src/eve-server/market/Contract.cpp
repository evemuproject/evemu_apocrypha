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
    Author:     Almamu, Aknor Jaden, adapted from /eve-server/src/character/Character.cpp authored by Zhur, Bloody.Rabbit
*/

#include "EVEServerPCH.h"

/*
 * ContractRequestItem
 */
ContractRequestItem::ContractRequestItem(
	uint32 _typeID,
	uint32 _quantity
	)
	: RefObject( 0 ),
	m_typeID(_typeID),
	m_quantity(_quantity)
{
}

ContractGetItems::ContractGetItems(
	uint32 _itemID,
	uint32 _quantity
	)
	: RefObject( 0 ),
	m_itemID( _itemID ),
	m_quantity( _quantity )
{
}

/*
 * ContractData
 */
ContractData::ContractData(
		uint32 _issuerID,
		uint32 _issuerCorpID, 
		uint32 _type,
		uint32 _avail, 
		uint32 _assigneeID,
		uint32 _expiretime, 
		uint32 _duration,
		uint32 _startStationID,
		uint32 _endStationID,
		uint32 _startSolarSystemID,
		uint32 _endSolarSystemID,
		uint32 _startRegionID,
		uint32 _endRegionID,
		double _price,
		double _reward,
		double _collateral,
		std::string _title,
		std::string _description,
		bool _forCorp,
		uint32 _status, 
		bool _isAccepted, 
		uint32 _acceptorID, 
		uint64 _dateIssued, 
		uint64 _dateExpired, 
		uint64 _dateAccepted, 
		uint64 _dateCompleted, 
		double _volume,
		bool _requiresAttention,
		uint32 _allianceID,
		uint32 _issuerWalletKey,
		uint32 _crateID
	)
	: 
  m_issuerID(_issuerID),
  m_issuerCorpID(_issuerCorpID),
  m_type(_type),
  m_avail(_avail),
  m_assigneeID(_assigneeID),
  m_expiretime(_expiretime),
  m_duration(_duration),
  m_startStationID(_startStationID),
  m_endStationID(_endStationID),
  m_startSolarSystemID(_startSolarSystemID),
  m_endSolarSystemID(_endSolarSystemID),
  m_startRegionID(_startRegionID),
  m_endRegionID(_endRegionID),
  m_price(_price),
  m_reward(_reward),
  m_collateral(_collateral),
  m_title(_title),
  m_description(_description),
  m_forCorp(_forCorp),
  m_status(_status),
  m_isAccepted(_isAccepted),
  m_acceptorID(_acceptorID),
  m_dateIssued(_dateIssued),
  m_dateExpired(_dateExpired),
  m_dateAccepted(_dateAccepted),
  m_dateCompleted(_dateCompleted),
  m_volume(_volume),
  m_requiresAttention(_requiresAttention),
  m_allianceID(_allianceID),
  m_issuerWalletKey(_issuerWalletKey),
  m_crateID(_crateID)
{

}

Contract::Contract(
		uint32 _contractID,
		ContractData &_contract,
		const std::map<uint32, ContractRequestItemRef> _requestItems,
		const std::map<uint32, ContractGetItemsRef> _items,
		ItemFactory &_itemFactory,
		ContractFactory &_factory
		)
: RefObject( 0 ),
  m_contractID(_contractID),
  m_contract( _contract ),
  m_requestItems(_requestItems),
  m_items(_items),
  m_itemFactory(_itemFactory),
  m_factory(_factory)
{
}

Contract::~Contract()
{

}

ContractRef Contract::Load( ItemFactory &item_factory, ContractFactory &contract_factory, uint32 contractID )
{
    return Contract::Load<Contract>( item_factory, contract_factory, contractID );
}

template<class _Ty>
RefPtr<_Ty> Contract::_LoadContract( ItemFactory &item_factory, ContractFactory &contract_factory, uint32 contractID,
        const std::map<uint32, ContractRequestItemRef> &contractRequestItems, const std::map<uint32, ContractGetItemsRef> &contractItems,
		ContractData &data
    )
{
    
    return ContractRef( new Contract( contractID, data, contractRequestItems, contractItems, item_factory, contract_factory ) );
}

void Contract::Delete()
{
	// Return the items to the hangar
	std::map<uint32, ContractGetItemsRef>::iterator cur, end;

	cur = items().begin();
	end = items().end();

	for(; cur != end; cur++)
	{
		InventoryItemRef item = m_itemFactory.GetItem( cur->second->m_itemID );
		item->Move( startStationID(), flagHangar, true );
		item->ChangeOwner( issuerID(), true );
	}

	// take ourself out of the DB
	m_factory.db().DeleteContract( contractID() );

	// Delete ourselves from factory cache
	m_factory.DeleteContract( contractID() );
}

bool Contract::_Load()
{
    // update inventory
    m_factory.AddContract( ContractRef( this ) );

    return true;
}

PyPackedRow* Contract::GetContractRow()
{
	DBRowDescriptor* header = new DBRowDescriptor;
	header->AddColumn( "contractID",			DBTYPE_I4 );
	header->AddColumn( "issuerID",				DBTYPE_I4 );
	header->AddColumn( "issuerCorpID",			DBTYPE_I4 );
	header->AddColumn( "type",					DBTYPE_UI1 );
	header->AddColumn( "availability",			DBTYPE_I4 );
	header->AddColumn( "assigneeID",			DBTYPE_I4 );
	header->AddColumn( "numDays",				DBTYPE_I4 );
	header->AddColumn( "startStationID",		DBTYPE_I4 );
	header->AddColumn( "endStationID",			DBTYPE_I4 );
	header->AddColumn( "startSolarSystemID",	DBTYPE_I4 );
	header->AddColumn( "endSolarSystemID",		DBTYPE_I4 );
	header->AddColumn( "startRegionID",			DBTYPE_I4 );
	header->AddColumn( "endRegionID",			DBTYPE_I4 );
	header->AddColumn( "price",					DBTYPE_CY );
	header->AddColumn( "reward",				DBTYPE_CY );
	header->AddColumn( "collateral",			DBTYPE_CY );
	header->AddColumn( "title",					DBTYPE_WSTR );
	header->AddColumn( "description",			DBTYPE_WSTR );
	header->AddColumn( "forCorp",				DBTYPE_BOOL );
	header->AddColumn( "status",				DBTYPE_UI1 );
	header->AddColumn( "acceptorID",			DBTYPE_I4 );
	header->AddColumn( "dateIssued",			DBTYPE_FILETIME );
	header->AddColumn( "dateExpired",			DBTYPE_FILETIME );
	header->AddColumn( "dateAccepted",			DBTYPE_FILETIME );
	header->AddColumn( "dateCompleted",			DBTYPE_FILETIME );
	header->AddColumn( "volume",				DBTYPE_R8 );
	header->AddColumn( "issuerAllianceID",		DBTYPE_I4 );
	header->AddColumn( "issuerWalletKey",		DBTYPE_I4 );
	header->AddColumn( "acceptorWalletKey",		DBTYPE_I4 );
	header->AddColumn( "crateID",				DBTYPE_I4 );

	PyPackedRow* row = new PyPackedRow( header );
	GetContractRow( row );
	return row;
}

void Contract::GetContractRow( PyPackedRow* into )
{
	into->SetField( "contractID",			new PyInt(		contractID() ) );
	into->SetField( "issuerID",				new PyInt(		issuerID() ) );
	into->SetField( "issuerCorpID",			new PyInt(		issuerCorpID() ) );
	into->SetField( "type",					new PyInt(		type() ) );
	into->SetField( "availability",			new PyInt(		avail() ) );
	into->SetField( "assigneeID",			new PyInt(		assigneeID() ) );
	into->SetField( "numDays",				new PyInt(		0 ) );
	into->SetField( "startStationID",		new PyInt(		startStationID() ) );
	into->SetField( "endStationID",			new PyInt(		endStationID() ) );
	into->SetField( "startSolarSystemID",   new PyInt(		startSolarSystemID() ) );
	into->SetField( "endSolarSystemID",		new PyInt(		endSolarSystemID() ) );
	into->SetField( "startRegionID",		new PyInt(		startRegionID() ) );
	into->SetField( "endRegionID",			new PyInt(		endRegionID() ) );
	into->SetField( "price",				new PyFloat(	price() ) );
	into->SetField( "reward",				new PyFloat(	reward() ) );
	into->SetField( "collateral",			new PyFloat(	collateral() ) );
	into->SetField( "title",				new PyWString(	"title", 5 ) );
	into->SetField( "description",			new PyWString(	"description", 11 ) );
	into->SetField( "forCorp",				new PyBool(		forCorp() ) );
	into->SetField( "status",				new PyInt(		status() ) );
	into->SetField( "acceptorID",			new PyInt(		acceptorID() ) );
	into->SetField( "dateIssued",			new PyLong(		dateIssued() ) );
	into->SetField( "dateExpired",			new PyLong(		dateExpired() ) );
	into->SetField( "dateAccepted",			new PyLong(		dateAccepted() ) );
	into->SetField( "dateCompleted",		new PyLong(		dateCompleted() ) );
	into->SetField( "volume",				new PyFloat(	volume() ) );
	into->SetField( "issuerAllianceID",		new PyInt(		issuerAllianceID() ) );
	into->SetField( "issuerWalletKey",		new PyInt(		issuerWalletKey() ) );
	into->SetField( "acceptorWalletKey",	new PyInt(		0 ) );
	into->SetField( "crateID",				new PyInt(		0 ) );
}


PyList* Contract::GetItemsList() const
{
	std::map<uint32, ContractGetItemsRef>::const_iterator cur, end;
	std::map<uint32, ContractGetItemsRef> item = items();
	PyList* res = new PyList;

	DBRowDescriptor* header = new DBRowDescriptor;
	header->AddColumn( "contractID",						DBTYPE_I4 );
	header->AddColumn( "itemID",							DBTYPE_I4 );
	header->AddColumn( "quantity",							DBTYPE_I4 );
	header->AddColumn( "itemTypeID",						DBTYPE_I4 );
	header->AddColumn( "inCrate",							DBTYPE_BOOL );
	header->AddColumn( "parentID",							DBTYPE_I4 );
	header->AddColumn( "productivityLevel",					DBTYPE_I4 );
	header->AddColumn( "materialLevel",						DBTYPE_I4 );
	header->AddColumn( "copy",								DBTYPE_I4 );
	header->AddColumn( "licensedProductionRunsRemaining",	DBTYPE_I4 );
	header->AddColumn( "damage",							DBTYPE_R8 );
	header->AddColumn( "flagID",							DBTYPE_I2 );

	cur = item.begin();
	end = item.end();

	for(; cur != end; *cur++ )
	{
		InventoryItemRef itemInfo = m_itemFactory.GetItem( cur->second->m_itemID );

		PyPackedRow* into = new PyPackedRow( header );
		GetItemRow( itemInfo, into );
		res->AddItem( into );
	}

	std::map<uint32, ContractRequestItemRef>::const_iterator c, e;
	std::map<uint32, ContractRequestItemRef> requestItem = requestItems();

	c = requestItem.begin();
	e = requestItem.end();

	for(; cur != end; *cur++ )
	{
		PyPackedRow* into = new PyPackedRow( header );
		GetRequestItemRow( c->second, into );
		res->AddItem( into );
	}

	return res;
}


void Contract::GetItemRow( InventoryItemRef item, PyPackedRow* into ) const
{
	into->SetField( "contractID",								new PyInt( contractID() ) );
	into->SetField( "itemID",									new PyInt( item->itemID() ) );
	into->SetField( "quantity",									new PyInt( item->quantity() ) );
	into->SetField( "itemTypeID",								new PyInt( item->typeID() ) );
	into->SetField( "inCrate",									new PyBool( true ) );

	if( item->categoryID() == EVEDB::invCategories::Blueprint )
	{
		BlueprintRef bp = m_itemFactory.GetBlueprint( item->itemID() );
		into->SetField( "parentID",								new PyInt( bp->parentBlueprintTypeID() ) );
		into->SetField( "productivityLevel",					new PyInt( bp->productivityLevel() ) );
		into->SetField( "materialLevel",						new PyInt( bp->materialLevel() ) );
		into->SetField( "copy",									new PyInt( bp->copy() ) );
		into->SetField( "licensedProductionRunsRemaining",		new PyInt( bp->licensedProductionRunsRemaining() ) );
	}
	else
	{
		into->SetField( "parentID",								new PyInt( 0 ) );
		into->SetField( "productivityLevel",					new PyInt( 0 ) );
		into->SetField( "materialLevel",						new PyInt( 0 ) );
		into->SetField( "copy",									new PyInt( 0 ) );
		into->SetField( "licensedProductionRunsRemaining",		new PyInt( 0 ) );
	}

	if( item->HasAttribute( AttrDamage ) )
		into->SetField( "damage",								new PyFloat( item->GetAttribute( AttrDamage ).get_float() ) );
	else
		into->SetField( "damage",								new PyFloat( 0.0 ) );

	into->SetField( "flagID",									new PyInt( item->flag() ) );
}


void Contract::GetRequestItemRow( ContractRequestItemRef item, PyPackedRow* into ) const
{
	into->SetField( "contractID",								new PyInt( contractID() ) );
	into->SetField( "itemID",									new PyInt( 0 ) );
	into->SetField( "quantity",									new PyInt( item->m_quantity ) );
	into->SetField( "itemTypeID",								new PyInt( item->m_typeID ) );
	into->SetField( "inCrate",									new PyBool( false ) );
	into->SetField( "parentID",									new PyInt( 0 ) );
	into->SetField( "productivityLevel",						new PyInt( 0 ) );
	into->SetField( "materialLevel",							new PyInt( 0 ) );
	into->SetField( "copy",										new PyInt( 0 ) );
	into->SetField( "licensedProductionRunsRemaining",			new PyInt( 0 ) );
	into->SetField( "damage",									new PyFloat( 0.0 ) );
	into->SetField( "flagID",									new PyInt( 0 ) );
}


CRowSet* Contract::GetBids() const
{
	DBRowDescriptor *bidsHeader = new DBRowDescriptor;
	bidsHeader->AddColumn( "bidID",					DBTYPE_I4 );
	bidsHeader->AddColumn( "contractID",			DBTYPE_I4 );
	bidsHeader->AddColumn( "issuerID",				DBTYPE_I4 );
	bidsHeader->AddColumn( "quantity",				DBTYPE_I4 );
	bidsHeader->AddColumn( "issuerCorpID",			DBTYPE_I4 );
	bidsHeader->AddColumn( "issuerStationID",		DBTYPE_I4 );
	bidsHeader->AddColumn( "issuerSolarSystemID",	DBTYPE_I4 );
	bidsHeader->AddColumn( "issuerRegionID",		DBTYPE_I4 );
	CRowSet *bids_rowset = new CRowSet( &bidsHeader );

	return bids_rowset;
}

