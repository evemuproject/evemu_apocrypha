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
    Author:     Aknor Jaden, adapted from /src/eve-server/character/Character.cpp authored by Zhur, Bloody.Rabbit
*/

#include "EVEServerPCH.h"


/*
 * Character
 */
Character::Character(
    ItemFactory &_factory,
    uint32 _characterID,
    // InventoryItem stuff:
    const CharacterType &_charType,
    const ItemData &_data,
    // Character stuff:
    const CharacterData &_charData,
    const CorpMemberInfo &_corpData)
: Owner(_factory, _characterID, _charType, _data),
  m_accountID(_charData.accountID),
  m_title(_charData.title),
  m_description(_charData.description),
  m_gender(_charData.gender),
  m_bounty(_charData.bounty),
  m_balance(_charData.balance),
  m_securityRating(_charData.securityRating),
  m_logonMinutes(_charData.logonMinutes),
  m_corporationID(_charData.corporationID),
  m_corpHQ(_corpData.corpHQ),
  m_allianceID(_charData.allianceID),
  m_warFactionID(_charData.warFactionID),
  m_corpRole(_corpData.corpRole),
  m_rolesAtAll(_corpData.rolesAtAll),
  m_rolesAtBase(_corpData.rolesAtBase),
  m_rolesAtHQ(_corpData.rolesAtHQ),
  m_rolesAtOther(_corpData.rolesAtOther),
  m_stationID(_charData.stationID),
  m_solarSystemID(_charData.solarSystemID),
  m_constellationID(_charData.constellationID),
  m_regionID(_charData.regionID),
  m_ancestryID(_charData.ancestryID),
  m_careerID(_charData.careerID),
  m_schoolID(_charData.schoolID),
  m_careerSpecialityID(_charData.careerSpecialityID),
  m_startDateTime(_charData.startDateTime),
  m_createDateTime(_charData.createDateTime),
  m_corporationDateTime(_charData.corporationDateTime)
{
    // allow characters to be only singletons
    assert(singleton() && quantity() == 1);
}

CharacterRef Character::Load(ItemFactory &factory, uint32 characterID)
{
    return InventoryItem::Load<Character>( factory, characterID );
}

template<class _Ty>
RefPtr<_Ty> Character::_LoadCharacter(ItemFactory &factory, uint32 characterID,
    // InventoryItem stuff:
    const CharacterType &charType, const ItemData &data,
    // Character stuff:
    const CharacterData &charData, const CorpMemberInfo &corpData)
{
    // construct the item
    return CharacterRef( new Character( factory, characterID, charType, data, charData, corpData ) );
}

CharacterRef Character::Spawn(ItemFactory &factory,
    // InventoryItem stuff:
    ItemData &data,
    // Character stuff:
    CharacterData &charData, CharacterAppearance &appData, CorpMemberInfo &corpData
) {
    uint32 characterID = Character::_Spawn( factory, data, charData, appData, corpData );
    if( characterID == 0 )
        return CharacterRef();

    CharacterRef charRef = Character::Load( factory, characterID );

    // Create default dynamic attributes in the AttributeMap:
    charRef.get()->SetAttribute(AttrIsOnline, 1);     // Is Online

    return charRef;
}

uint32 Character::_Spawn(ItemFactory &factory,
    // InventoryItem stuff:
    ItemData &data,
    // Character stuff:
    CharacterData &charData, CharacterAppearance &appData, CorpMemberInfo &corpData
) {
    // make sure it's a character
    const CharacterType *ct = factory.GetCharacterType(data.typeID);
    if(ct == NULL)
        return 0;

    // make sure it's a singleton with qty 1
    if(!data.singleton || data.quantity != 1) {
        _log(ITEM__ERROR, "Tried to create non-singleton character %s.", data.name.c_str());
        return 0;
    }

    // first the item
    uint32 characterID = Owner::_Spawn(factory, data);
    if(characterID == 0)
        return 0;

    // then character
    if(!factory.db().NewCharacter(characterID, charData, appData, corpData)) {
        // delete the item
        factory.db().DeleteItem(characterID);

        return 0;
    }

    return characterID;
}

bool Character::_Load()
{
    if( !LoadContents( m_factory ) )
        return false;

    if( !m_factory.db().LoadSkillQueue( itemID(), m_skillQueue ) )
        return false;

    // Calculate total SP trained and store in internal variable:
    _CalculateTotalSPTrained();

    return Owner::_Load();
}

void Character::Delete() {
    // delete contents
    DeleteContents( m_factory );

    // delete character record
    m_factory.db().DeleteCharacter(itemID());

    // let the parent care about the rest
    Owner::Delete();
}

void Character::SetDescription(const char *newDescription) {
    m_description = newDescription;
}

PyObject *Character::GetDescription() const
{
    util_Row row;

    row.header.push_back("description");

    row.line = new PyList;
    row.line->AddItemString( description().c_str() );

    return row.Encode();
}

void Character::AddItem(InventoryItemRef item)
{
    Inventory::AddItem( item );
}

