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
	Author:		Aknor Jaden, adapted from /eve-server/src/character/Character.cpp authored by Zhur, Bloody.Rabbit
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
class Contract
: public Inventory
{
	friend class InventoryItem;	// to let it construct us
public:
	typedef InventoryDB::QueuedSkill QueuedSkill;
	typedef InventoryDB::SkillQueue SkillQueue;

	/**
	 * Loads contract.
	 *
	 * @param[in] factory
	 * @param[in] contractD ID of contract to load.
	 * @return Pointer to new Contract object; NULL if failed.
	 */
	static ContractRef Load(ItemFactory &factory, uint32 contractID);
	/**
	 * Spawns new contract.
	 *
	 * @param[in] factory
	 * @return Pointer to new Contract object; NULL if failed.
	 */
	static ContractRef Spawn(ItemFactory &factory);

	/*
	 * Primary public interface:
	 */
	// CreateContract();
	// AcceptContract();
	// RevokeContract();

	void Delete();


	/*
	 * Public fields:
	 */
    // General information:
    const uint32            contractID() const { return inventoryID(); }
	const std::string &     title() const { return m_title; }
	const std::string &     description() const { return m_description; }

    // Contract Owner information:
	uint32                  ownerCharacterID() const { return m_ownerCharacterID; }
	uint32                  ownerCorporationID() const { return m_ownerCorporationID; }

	// Some importand dates:
	uint64                  createDateTime() const { return m_createDateTime; }

protected:
	Contract(
        ItemFactory &_factory,
        uint32 _contractID,
        std::string _title,
        std::string _description,
        uint32 _ownerCharacterID,
        uint32 _ownerCorporationID,
        uint64 _createDateTime)
	);

	/*
	 * Member functions:
	 */

	// Actual loading stuff:
	template<class _Ty>
	static RefPtr<_Ty> _LoadContract(ItemFactory &factory, uint32 characterID);

	bool _Load();

	static uint32 _Spawn(ItemFactory &factory);

	uint32 inventoryID() const { return m_contractID; }
	PyRep *GetItem() const { return new PyNone(); }

	void AddItem(InventoryItemRef item);

	/*
	 * Data members
	 */
    uint32 m_contractID;

	std::string m_title;
	std::string m_description;

    uint32 m_ownerCharacterID;
	uint32 m_ownerCorporationID;

	uint64 m_createDateTime;
};

#endif /* !__CHARACTER__H__INCL__ */

