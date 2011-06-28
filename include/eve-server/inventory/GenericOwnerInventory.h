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
	Author:		Aknor Jaden, adapted from /include/eve-server/character/Character.h authored by Zhur, Bloody.Rabbit
*/

#ifndef __GENERICOWNERINVENTORY__H__INCL__
#define __GENERICOWNERINVENTORY__H__INCL__


#include "inventory/ItemType.h"
#include "inventory/Owner.h"
#include "inventory/Inventory.h"
#include "inventory/InventoryDB.h"

/**
 * Class representing Generic Owner Inventory.
 */
class GenericOwnerInventory
: public Owner,
  public Inventory
{
	friend class InventoryItem;	// to let it construct us
	friend class Owner;	// to let it construct us
public:

	/**
	 * Loads GenericOwnerInventory.
	 *
	 * @param[in] factory
	 * @param[in] inventoryID ID of GenericOwnerInventory to load.
	 * @return Pointer to new GenericOwnerInventory object; NULL if failed.
	 */
	static GenericOwnerInventoryRef Load(ItemFactory &factory, uint32 inventoryID);

	/**
	 * Spawns new GenericOwnerInventory.
	 *
	 * @param[in] factory
	 * @param[in] data ItemData (data for entity table) for new GenericOwnerInventory.
	 * @return Pointer to new GenericOwnerInventory object; NULL if failed.
	 */
	static GenericOwnerInventoryRef Spawn(ItemFactory &factory, ItemData &data);

	/*
	 * Primary public interface:
	 */
	void SetDescription(const char *newDescription);

	void Delete();


	// NOTE: We do not handle Split/Merge logic since singleton-restricted construction does this for us.


	/*
	 * Primary public packet builders:
	 */
	PyObject *GetDescription() const;

	/*
	 * Public fields:
	 */
	// Account:
	uint32                  inventoryID() const { return m_inventoryID; }

	const std::string &     title() const { return m_title; }
	const std::string &     description() const { return m_description; }

    // Owner Information:
    uint32                  ownerCharacterID() const { return m_ownerCharacterID; }
	uint32                  ownerCorporationID() const { return m_ownerCorporationID; }

	uint64                  createDateTime() const { return m_createDateTime; }

protected:
	Character(
		ItemFactory &_factory,
		uint32 _characterID,
		// InventoryItem stuff:
		const ItemData &_data
	);

	/*
	 * Member functions:
	 */
	using InventoryItem::_Load;

	// Template loader:
	template<class _Ty>
	static RefPtr<_Ty> _LoadOwner(ItemFactory &factory, uint32 characterID,
		// InventoryItem stuff:
		const ItemType &type, const ItemData &data)
	{
		// check it's a character
		if( type.groupID() != EVEDB::invGroups::Character )
		{
			sLog.Error("Character", "Trying to load %s as Character.", type.group().name().c_str() );
			return RefPtr<_Ty>();
		}
		// cast the type
		const CharacterType &charType = static_cast<const CharacterType &>( type );

		CharacterData charData;
		if( !factory.db().GetCharacter( characterID, charData ) )
			return RefPtr<_Ty>();

		CorpMemberInfo corpData;
		if( !factory.db().GetCorpMemberInfo( characterID, corpData ) )
			return RefPtr<_Ty>();

		return _Ty::template _LoadCharacter<_Ty>( factory, characterID, charType, data, charData, corpData );
	}

	// Actual loading stuff:
	template<class _Ty>
	static RefPtr<_Ty> _LoadCharacter(ItemFactory &factory, uint32 characterID,
		// InventoryItem stuff:
		const CharacterType &charType, const ItemData &data,
		// Character stuff:
		const CharacterData &charData, const CorpMemberInfo &corpData
	);

	bool _Load();

	static uint32 _Spawn(ItemFactory &factory,
		// InventoryItem stuff:
		ItemData &data,
		// Character stuff:
		CharacterData &charData, CharacterAppearance &appData, CorpMemberInfo &corpData
	);

	uint32 inventoryID() const { return itemID(); }
	PyRep *GetItem() const { return GetItemRow(); }

	void AddItem(InventoryItemRef item);

    void _CalculateTotalSPTrained();

	/*
	 * Data members
	 */
	uint32 m_accountID;

	std::string m_title;
	std::string m_description;
	bool m_gender;

	double m_bounty;
	double m_balance;
	double m_securityRating;
	uint32 m_logonMinutes;

	uint32 m_corporationID;
	uint32 m_corpHQ;
	uint32 m_allianceID;
	uint32 m_warFactionID;

	uint64 m_corpRole;
	uint64 m_rolesAtAll;
	uint64 m_rolesAtBase;
	uint64 m_rolesAtHQ;
	uint64 m_rolesAtOther;

	uint32 m_stationID;
	uint32 m_solarSystemID;
	uint32 m_constellationID;
	uint32 m_regionID;

	uint32 m_ancestryID;
	uint32 m_careerID;
	uint32 m_schoolID;
	uint32 m_careerSpecialityID;

	uint64 m_startDateTime;
	uint64 m_createDateTime;
	uint64 m_corporationDateTime;

	// Skill queue:
	SkillQueue m_skillQueue;
    EvilNumber m_totalSPtrained;
};

#endif /* !__CHARACTER__H__INCL__ */
