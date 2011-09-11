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
	Author:		Almamu, Zhur
*/


#include "EVEServerPCH.h"


PyCallable_Make_InnerDispatcher(InsuranceService)


class InsuranceBound
: public PyBoundObject {
public:

	PyCallable_Make_Dispatcher(InsuranceBound)
	
	InsuranceBound(PyServiceMgr *mgr, InsuranceDB *db)
	: PyBoundObject(mgr),
	  m_db(db),
	  m_dispatch(new Dispatcher(this))
	{
		_SetCallDispatcher(m_dispatch);
		
		m_strBoundObjectName = "InsuranceBound";

		PyCallable_REG_CALL(InsuranceBound, GetContracts)
		PyCallable_REG_CALL(InsuranceBound, InsureShip)
		PyCallable_REG_CALL(InsuranceBound, UnInsureShip)
	}
	virtual ~InsuranceBound() { delete m_dispatch; }
	virtual void Release() {
		//I hate this statement
		delete this;
	}
	
	PyCallable_DECL_CALL(GetContracts)
	PyCallable_DECL_CALL(InsureShip)
	PyCallable_DECL_CALL(UnInsureShip)

protected:
	InsuranceDB *const m_db;
	Dispatcher *const m_dispatch;
};



InsuranceService::InsuranceService(PyServiceMgr *mgr)
: PyService(mgr, "insuranceSvc"),
  m_dispatch(new Dispatcher(this))
{
	_SetCallDispatcher(m_dispatch);

	PyCallable_REG_CALL(InsuranceService, GetContractForShip)
	//PyCallable_REG_CALL(InsuranceService, )
}

InsuranceService::~InsuranceService() {
	delete m_dispatch;
}



PyBoundObject* InsuranceService::_CreateBoundObject( Client* c, const PyRep* bind_args )
{
	_log( CLIENT__MESSAGE, "InsuranceService bind request for:" );
	bind_args->Dump( CLIENT__MESSAGE, "    " );

	return new InsuranceBound( m_manager, &m_db );
}


PyResult InsuranceService::Handle_GetContractForShip( PyCallArgs& call )
{
    Call_SingleIntegerArg arg;
	
	if( !arg.Decode( call.tuple ) )
	{
		_log(SERVICE__ERROR, "Wrong parameters to GetContractForShip." );
		return new PyNone;
	}

	uint32 characterID = call.client->GetCharacterID();
	std::vector<int32> ships;
	
	ships.push_back( arg.arg );

    return m_db.GetContracts( ships, characterID );
}


PyResult InsuranceBound::Handle_GetContracts( PyCallArgs& call )
{

	uint32 stationID = call.client->GetStationID();
	uint32 characterID = call.client->GetCharacterID();

	std::vector<int32> items;
	std::vector<int32> ships;

	// Get all the items in the current hangar
	if( m_db->GetStationItems( items, stationID, characterID ) == false )
	{
		return new PyNone;
	}

	// Select only ships and add them to ships vector
	for( size_t i = 0; i < items.size(); i ++ )
	{
		InventoryItemRef item = m_manager->item_factory.GetItem( items.at( i ) );

		if( item->categoryID() == EVEDB::invCategories::Ship )
		{
			ships.push_back( item->itemID() );
		}
	}

	return m_db->GetContracts( ships, characterID );
}


PyResult InsuranceBound::Handle_InsureShip( PyCallArgs& call )
{
	Call_InsureShip args;

	if( !args.Decode( call.tuple ) )
	{
		_log(SERVICE__ERROR, "Wrong parameters to InsureShip" );
		return new PyBool( false );
	}

	printf( "shipID: %d cost: %f type: %d\n", args.shipID, args.cost, args.type );

	double baseprice = m_manager->item_factory.GetItem( args.shipID )->type().basePrice();
	double fraction = args.cost * 100 / baseprice;

	if( call.client->GetChar()->balance() > args.cost )
		call.client->AddBalance( -args.cost );
	else
	{
		call.client->SendInfoModalMsg( "You don't have enough ISK to insure this ship with this insurance type." );
		return new PyBool( false );
	}

	if( m_manager->item_factory.GetItem( args.shipID )->singleton() == false )
	{
		call.client->SendInfoModalMsg( "You can't insure repackaged ships..." );
		return new PyBool( false );
	}
	
	return new PyBool( m_db->InsureShip( args.shipID, call.client->GetCharacterID(), fraction / 5 ) );
}


PyResult InsuranceBound::Handle_UnInsureShip( PyCallArgs& call )
{
	Call_SingleIntegerArg arg;

	if( !arg.Decode( call.tuple ) )
	{
		_log(SERVICE__ERROR, "Wrong parameters to UnInsureShip" );
		return new PyBool( false );
	}

	return new PyBool( m_db->UnInsureShip( arg.arg, call.client->GetCharacterID() ) );
}


























