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
    Author:     Aknor Jaden, adapted from /eve-server/src/character/Character.cpp authored by Zhur, Bloody.Rabbit
*/

#include "EVEServerPCH.h"


/*
 * Contract
 */
Contract::Contract(
    ItemFactory &_factory,
    uint32 _contractID,
    std::string _title,
    std::string _description,
    uint32 _ownerCharacterID,
    uint32 _ownerCorporationID,
    uint64 _createDateTime)
: m_contractID(_contractID),
  m_title(_title),
  m_description(_description),
  m_ownerCharacterID(_ownerCharacterID),
  m_ownerCorporationID(_ownerCorporationID),
  m_createDateTime(_createDateTime)
{
    // allow contracts to be only singletons
    assert(singleton() && quantity() == 1);
}

ContractRef Contract::Load(ItemFactory &factory, uint32 contractID)
{
    return _LoadContract<Contract>( factory, contractID );
}

template<class _Ty>
RefPtr<_Ty> Contract::_LoadContract(ItemFactory &factory, uint32 contractID)
{
    // construct the item
    return ContractRef( new Contract( factory, contractID ) );
}

ContractRef Contract::Spawn(ItemFactory &factory) {
    uint32 contractID = Contract::_Spawn( factory, data, charData, appData, corpData );
    if( contractID == 0 )
        return CharacterRef();

    ContractRef contractRef = Contract::Load( factory, contractID );

    return contractRef;
}

uint32 Contract::_Spawn(ItemFactory &factory) {
    // make sure it's a singleton with qty 1
    if(!data.singleton || data.quantity != 1) {
        _log(ITEM__ERROR, "Tried to create non-singleton Contract %s.", data.name.c_str());
        return 0;
    }

    // first the item
    uint32 contractID = Owner::_Spawn(factory, data);
    if(contractID == 0)
        return 0;

    // then contract
    if(!factory.db().NewContract(contractID)) {
        // delete the item
        factory.db().DeleteItem(contractID);

        return 0;
    }

    return contractID;
}

bool Contract::_Load()
{
    if( !LoadContents( m_factory ) )
        return false;

    return true;
}

void Contract::Delete() {
    // delete contents
    DeleteContents( m_factory );

    // delete Contract record
    m_factory.db().DeleteContract(itemID());
}

void Contract::SetDescription(const char *newDescription) {
    m_description = newDescription;

    SaveContract();
}

PyObject *Contract::GetDescription() const
{
    util_Row row;

    row.header.push_back("description");

    row.line = new PyList;
    row.line->AddItemString( description().c_str() );

    return row.Encode();
}

void Contract::AddItem(InventoryItemRef item)
{
    Inventory::AddItem( item );
}

void Contract::SaveContract()
{
    _log( ITEM__TRACE, "Saving Contract %u.", itemID() );

    sLog.Debug( "Contract::SaveContract()", "Saving all Contract info and items list to DB for Contract %d...", m_contractID );
    // character data
    m_factory.db().SaveContract(
        itemID(),
    );
}

