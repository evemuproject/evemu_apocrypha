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
class ContractDB;

class ContractRequestItem
{
public:
	ContractRequestItem(
		uint32 _typeID,
		uint32 _quantity);

	uint32 m_typeID;
	uint32 m_quantity;
};


// Contracts are NOT items
class ContractData
{
public:
	ContractData(
		uint32 _contractID,
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
		uint32 _crateID,
		uint64 _lastChange
	);

	uint32 m_contractID;
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
	uint64 m_lastChange;
};

class Contract
{
	// friend class InventoryItem;	// to let it construct us
public:
	typedef InventoryDB::QueuedSkill QueuedSkill;
	typedef InventoryDB::SkillQueue SkillQueue;

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
	 *
	static ContractRef Load( uint32 contractID );
	/**
	 * Spawns new contract.
	 *
	 * @param[in] factory
	 * @return Pointer to new Contract object; NULL if failed.
	 *
	static ContractRef Spawn( );

	/*
	 * Primary public interface:
	 */
	// CreateContract();
	// AcceptContract();
	// RevokeContract();

	//void Delete();


	/*
	 * Public fields:
	 */
    // General information:
    const uint32            contractID() const { return m_contract.m_contractID; }
	const std::string &     title() const { return m_contract.m_title; }
	const std::string &     description() const { return m_contract.m_description; }

    // Contract Owner information:
	uint32                  issuerID() const { return m_contract.m_issuerID; }
	uint32                  issuerCorpID() const { return m_contract.m_issuerCorpID; }

	// Some importand dates:
	uint64                  dateIssued() const { return m_contract.m_dateIssued; }

	Contract(
		ContractData &_contract,
		std::map<uint32, ContractRequestItem> _requestItemTypeList,
		std::map<uint32, InventoryItemRef> _itemList
		);

	ContractData &m_contract;
	std::map<uint32, ContractRequestItem> m_requestItemTypeList;
	std::map<uint32, InventoryItemRef> m_itemList;

protected:

};

#endif /* !__CONTRACT__H__INCL__ */

