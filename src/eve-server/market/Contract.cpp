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




