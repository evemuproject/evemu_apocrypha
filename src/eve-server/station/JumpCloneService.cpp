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

PyCallable_Make_InnerDispatcher(JumpCloneService)

class JumpCloneBound
: public PyBoundObject {
public:
	
	PyCallable_Make_Dispatcher(JumpCloneBound)
	
	JumpCloneBound(PyServiceMgr *mgr, StationDB *db)
	: PyBoundObject(mgr),
	  m_db(db),
	  m_dispatch(new Dispatcher(this))
	{
		_SetCallDispatcher(m_dispatch);

        m_strBoundObjectName = "JumpCloneBound";
		
		PyCallable_REG_CALL(JumpCloneBound, GetCloneState)
		PyCallable_REG_CALL(JumpCloneBound, InstallCloneInStation)
		PyCallable_REG_CALL(JumpCloneBound, GetPriceForClone)
		PyCallable_REG_CALL(JumpCloneBound, CloneJump)
		PyCallable_REG_CALL(JumpCloneBound, DestroyInstalledClone)
		PyCallable_REG_CALL(JumpCloneBound, GetShipCloneState)
	}
	virtual ~JumpCloneBound() { delete m_dispatch; }
	virtual void Release() {
		//I hate this statement
		delete this;
	}
	
	PyCallable_DECL_CALL(GetCloneState)
	PyCallable_DECL_CALL(InstallCloneInStation)
	PyCallable_DECL_CALL(GetPriceForClone)
	PyCallable_DECL_CALL(CloneJump)
	PyCallable_DECL_CALL(DestroyInstalledClone)
	PyCallable_DECL_CALL(GetShipCloneState)

protected:
	StationDB *const m_db;		//we do not own this
	Dispatcher *const m_dispatch;	//we own this
};



JumpCloneService::JumpCloneService(PyServiceMgr *mgr)
: PyService(mgr, "jumpCloneSvc"),
  m_dispatch(new Dispatcher(this))
{
	_SetCallDispatcher(m_dispatch);
	
//	PyCallable_REG_CALL(JumpCloneService, )
}

JumpCloneService::~JumpCloneService() {
	delete m_dispatch;
}

PyBoundObject* JumpCloneService::_CreateBoundObject( Client* c, const PyRep* bind_args )
{
	_log( CLIENT__MESSAGE, "JumpCloneService bind request for:" );
	bind_args->Dump( CLIENT__MESSAGE, "    " );

	return new JumpCloneBound( m_manager, &m_db );
}

PyResult JumpCloneBound::Handle_InstallCloneInStation( PyCallArgs& call )
{
	//takes no arguments, returns no arguments

    sLog.Debug( "JumpCloneBound", "Called InstallCloneInStation stub." );
	
    return m_db->InstallClone( call.client->GetCharacterID(), call.client->GetStationID(), call.client );
}

PyResult JumpCloneBound::Handle_GetCloneState(PyCallArgs &call) {
	
	//returns (clones, implants, timeLastJump)
	//where jumpClones is a rowset? with at least columns: jumpCloneID, locationID

    sLog.Debug( "JumpCloneBound", "Called GetCloneState stub." );

	PyDict* d = new PyDict;
	d->SetItemString( "clones", m_db->GetCharacterClones( call.client->GetCharacterID() ) );
	d->SetItemString( "implants", new PyNone );
	d->SetItemString( "timeLastJump", new PyNone ); // Free clone jumping!!!

	return new PyObject(
        new PyString( "util.KeyVal" ), d
    );
}


PyResult JumpCloneBound::Handle_GetPriceForClone(PyCallArgs &call) {
	return new PyFloat( 1125000 ); // Harcoded
}

PyResult JumpCloneBound::Handle_CloneJump( PyCallArgs &call )
{
	// What should we do here ?
	sLog.Debug( "JumpCloneBound", "Called CloneJump stub" );
	return new PyBool( true ); // Lets try this
}

PyResult JumpCloneBound::Handle_DestroyInstalledClone( PyCallArgs &call )
{
	Call_SingleIntegerArg arg;
	sLog.Debug( "JumpCloneBound", "Called DestroyInstalledClone stub" );
	
	if( !arg.Decode( &call.tuple ) )
	{
		sLog.Error( "JumpCloneBound" ,"Wrong parameters to DestroyInstalledClone");
		return NULL;
	}

	return m_db->DestroyClone( call.client->GetCharacterID(), arg.arg, call.client->GetStationID(), call.client );
}

PyResult JumpCloneBound::Handle_GetShipCloneState( PyCallArgs &call )
{
	sLog.Debug( "JumpCloneBound", "Called GetShipCloneState stub" );
	
	return NULL; // Harcoded, what should we do here?
}

/*
PyResult JumpCloneService::Handle_(PyCallArgs &call) {
	PyRep *result = NULL;

	return result;
}*/
























