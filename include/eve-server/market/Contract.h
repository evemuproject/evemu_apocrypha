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
	Author:		Almamu, Aknor Jaden, adapted from /eve-server/src/character/Character.cpp authored by Zhur, Bloody.Rabbit
*/

#ifndef __CONTRACT__H__INCL__
#define __CONTRACT__H__INCL__

#include "inventory/ItemType.h"
#include "inventory/Owner.h"
#include "inventory/Inventory.h"
#include "inventory/InventoryDB.h"


/**
 * Class representing contract.
 */
class Skill;
class ContractFactory;

class ContractRequestItem
	: public RefObject
{
public:
	ContractRequestItem(
		uint32 _typeID,
		uint32 _quantity);

	uint32 m_typeID;
	uint32 m_quantity;
};

class ContractGetItems
	: public RefObject
{
public:
	ContractGetItems(
		uint32 _itemID,
		uint32 quantity);

	uint32 m_itemID;
	uint32 m_quantity;
};


class ContractData
{
public:
	ContractData(
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
	);

	uint32 m_issuerID;
	uint32 m_issuerCorpID; 
	uint32 m_type; 
	uint32 m_avail; 
	uint32 m_assigneeID; 
	uint32 m_expiretime; 
	uint32 m_duration; 
	uint32 m_startStationID; 
	uint32 m_endStationID; 
	uint32 m_startSolarSystemID; 
	uint32 m_endSolarSystemID; 
	uint32 m_startRegionID; 
	uint32 m_endRegionID; 
	double m_price; 
	double m_reward; 
	double m_collateral; 
	std::string m_title; 
	std::string m_description; 
	bool m_forCorp; 
	uint32 m_status; 
	bool m_isAccepted; 
	uint32 m_acceptorID; 
	uint64 m_dateIssued; 
	uint64 m_dateExpired; 
	uint64 m_dateAccepted; 
	uint64 m_dateCompleted; 
	double m_volume; 
	bool m_requiresAttention;
	uint32 m_allianceID;
	uint32 m_issuerWalletKey;
	uint32 m_crateID;
};

class Contract
	: public RefObject
{
	// friend class InventoryItem;	// to let it construct us
public:
	Contract(
		uint32 _contractID,
		ContractData &_contract,
		const std::map<uint32, ContractRequestItemRef> _requestItems,
		const std::map<uint32, ContractGetItemsRef> _items,
		ItemFactory &_itemFactory,
		ContractFactory &_factory
	);

	/*
	 * Loads a contract
	 *
	 * @param[in] contractID
	 * @return Pointer to new Contract object, NULL if failed
	 *
	static ContractRef Load( uint32 contractID );
	/**
	 * Loads contract.
	 *
	 * @param[in] factory
	 * @param[in] contractD ID of contract to load.
	 * @return Pointer to new Contract object; NULL if failed.
	 */
	static ContractRef Load( ItemFactory &item_factory, ContractFactory &contract_factory, uint32 contractID );
	/**
	 * Spawns new contract.
	 *
	 * @param[in] factory
	 * @return Pointer to new Contract object; NULL if failed.
	 *
	static ContractRef Spawn( );

	/*
	 * Public fields:
	 */

	virtual void Delete();

    // General information:
    uint32		            contractID() const { return m_contractID; }
	std::string &		    title() const { return m_contract.m_title; }
	std::string &			description() const { return m_contract.m_description; }
	uint32					type() const { return m_contract.m_type; }
	uint32					avail() const { return m_contract.m_avail; }

	uint32					assigneeID() const { return m_contract.m_assigneeID; }
	
	// Money info
	double					price() const { return m_contract.m_price; }
	double					reward() const { return m_contract.m_reward; }
	double					collateral() const { return m_contract.m_collateral; }

    // Contract Owner information:
	uint32                  issuerID() const { return m_contract.m_issuerID; }
	uint32                  issuerCorpID() const { return m_contract.m_issuerCorpID; }
	uint32					issuerAllianceID() const { return m_contract.m_allianceID; }

	// Some importand dates:
	uint64                  dateIssued() const { return m_contract.m_dateIssued; }
	uint64					dateCompleted() const { return m_contract.m_dateCompleted; }
	uint64					dateAccepted() const { return m_contract.m_dateAccepted; }
	uint64					dateExpired() const { return m_contract.m_dateExpired; }
	uint32					expiretime() const { return m_contract.m_expiretime; }
	uint32					duration() const { return m_contract.m_duration; }

	// Location info
	uint32					startStationID() const { return m_contract.m_startStationID; }
	uint32					endStationID() const { return m_contract.m_endStationID; }
	uint32					startSolarSystemID() const { return m_contract.m_startSolarSystemID; }
	uint32					endSolarSystemID() const { return m_contract.m_endSolarSystemID; }
	uint32					startRegionID() const { return m_contract.m_startRegionID; }
	uint32					endRegionID() const { return m_contract.m_endRegionID; }

	bool					forCorp() const { return m_contract.m_forCorp; }
	uint32					status() const { return m_contract.m_status; }
	bool					isAccepted() const { return m_contract.m_isAccepted; }
	uint32					acceptorID() const { return m_contract.m_acceptorID; }
	double					volume() const { return m_contract.m_volume; }
	uint32					issuerWalletKey() const { return m_contract.m_issuerWalletKey; }
	bool					requiresAttention() const { return m_contract.m_requiresAttention; }

	ContractData			contractData() const { return m_contract; }
	ItemFactory&			itemFactory() const { return m_itemFactory; }
	ContractFactory&		contractFactory() const { return m_factory; }

	std::map<uint32, ContractRequestItemRef> requestItems() const{ return m_requestItems; }
	std::map<uint32, ContractGetItemsRef> items() const { return m_items; }

	PyPackedRow* GetContractRow();
	void GetContractRow( PyPackedRow* into );

	PyList* GetItemsList() const;
	void GetItemRow( InventoryItemRef item, PyPackedRow* into ) const;
	void GetRequestItemRow( ContractRequestItemRef item, PyPackedRow* into ) const;

	CRowSet* GetBids() const;
protected:
	virtual ~Contract();

	// Template helper
    template<class _Ty>
    static RefPtr<_Ty> Load( ItemFactory &item_factory, ContractFactory &contract_factory, uint32 contractID )
    {
        // static load
        RefPtr<_Ty> i = _Ty::template _Load<_Ty>( item_factory, contract_factory, contractID );
        if( !i )
            return RefPtr<_Ty>();

        // dynamic load
        if( !i->_Load() )
            return RefPtr<_Ty>();

        return i;
    }

    // Template loader:
    template<class _Ty>
    static RefPtr<_Ty> _Load( ItemFactory &item_factory, ContractFactory &contract_factory, uint32 contractID )
    {
        // pull the contract data
        ContractData *data = contract_factory.db().GetContractInfo( contractID );
        if( data == NULL )
            return RefPtr<_Ty>();

		std::map<uint32, ContractRequestItemRef> _rItems;
		std::map<uint32, ContractGetItemsRef> _Items;
        contract_factory.db().GetContractItems( contractID,  _Items, _rItems );

        return _Ty::template _LoadContract<_Ty>( item_factory, contract_factory, contractID, _rItems, _Items, *data );
    }

    // Actual loading stuff:
	template<class _Ty>
	static RefPtr<_Ty> _LoadContract( ItemFactory &item_factory, ContractFactory &contract_factory, uint32 contractID,
        const std::map<uint32, ContractRequestItemRef> &contractRequestItems, const std::map<uint32, ContractGetItemsRef> &contractItems,
		ContractData &data
    );

	virtual bool _Load();

	// our contract factory
	ContractFactory& m_factory;

	// contract data
	ContractData &m_contract;

	// item factory
	ItemFactory &m_itemFactory;

	const uint32 m_contractID;

	// contract items
	const std::map<uint32, ContractRequestItemRef> m_requestItems;
	const std::map<uint32, ContractGetItemsRef> m_items;
};

#endif /* !__CONTRACT__H__INCL__ */

