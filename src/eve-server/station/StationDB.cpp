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

storage StationDB::thingy;

StationDB::StationDB()
{
	thingy.load();
}

PyRep *StationDB::GetSolarSystem(uint32 solarSystemID) {
	
	return thingy.find(solarSystemID)->Clone();

	// old code for reference.
	/*DBQueryResult res;
	
	if(!sDatabase.RunQuery(res,
		"SELECT "
		" solarSystemID,"			// nr
		" solarSystemName,"			// string
		" x, y, z,"					// double
		" radius,"					// double
		" security,"				// double
		" constellationID,"			// nr
		" factionID,"				// nr
		" sunTypeID,"				// nr
		" regionID,"
		//crap
		" NULL AS allianceID,"		// nr
		" 0 AS sovereigntyLevel,"	// nr
		" 0 AS constellationSovereignty"	// nr
		" FROM mapSolarSystems"		
		" WHERE solarSystemID=%u", solarSystemID
	))
	{
		_log(SERVICE__ERROR, "Error in GetSolarSystem query: %s", res.error.c_str());
		return NULL;
	}
	
	DBResultRow row;
	if(!res.GetRow(row)) {
		_log(SERVICE__ERROR, "Error in GetSolarSystem query: no solarsystem for id %d", solarSystemID);
		return NULL;
	}
	
	return(DBRowToRow(row));*/
}

PyRep *StationDB::DoGetStation(uint32 sid) {
	DBQueryResult res;

	if(!sDatabase.RunQuery(res,
		" SELECT "
		" staStations.stationID, staStations.security, staStations.dockingCostPerVolume, staStations.maxShipVolumeDockable, "
		" staStations.officeRentalCost, staStations.operationID, staStations.stationTypeID, staStations.corporationID AS ownerID, staStations.solarSystemID, staStations.constellationID, "
		" staStations.regionID, staStations.stationName, staStations.x, staStations.y, staStations.z, staStations.reprocessingEfficiency, staStations.reprocessingStationsTake, staStations.reprocessingHangarFlag, "
		" staOperations.description, "
		// damn mysql returns the result of the sum as string and so it is sent to the client as string and so it freaks out...
		" CAST(SUM(staOperationServices.serviceID) as UNSIGNED INTEGER) AS serviceMask "
		" FROM staStations "
		" LEFT JOIN staOperations ON staStations.operationID = staOperations.operationID "
		" LEFT JOIN staOperationServices ON staStations.operationID = staOperationServices.operationID "
		" WHERE staStations.stationID = %u "
		" GROUP BY staStations.stationID ", sid
	))
	{
		_log(SERVICE__ERROR, "Error in DoGetStation query: %s", res.error.c_str());
		return NULL;
	}

	DBResultRow row;
	if(!res.GetRow(row)) {
		_log(SERVICE__ERROR, "Error in DoGetStation query: no station for id %d", sid);
		return NULL;
	}

	//only a guess that this is row
	return(DBRowToKeyVal(row));
}

//hangarGraphicID
//station owner corp
//station ID
//serviceMask
//stationTypeID

PyRep *StationDB::GetStationItemBits(uint32 sid) {
	DBQueryResult res;

	if(!sDatabase.RunQuery(res,
		" SELECT "
		" staStations.stationID, "
		" staStations.stationTypeID, staStations.corporationID AS ownerID, "
		" staStationTypes.hangarGraphicID, "
		// damn mysql returns the result of the sum as string and so it is sent to the client as string and so it freaks out...
		" CAST(SUM(staOperationServices.serviceID) as UNSIGNED INTEGER) AS serviceMask "
		" FROM staStations "
		" LEFT JOIN staStationTypes ON staStations.stationTypeID = staStationTypes.stationTypeID "
		" LEFT JOIN staOperationServices ON staStations.operationID = staOperationServices.operationID "
		" WHERE staStations.stationID = %u "
		" GROUP BY staStations.stationID ", sid
	))
	{
		_log(SERVICE__ERROR, "Error in DoGetStation query: %s", res.error.c_str());
		return NULL;
	}

	DBResultRow row;
	if(!res.GetRow(row)) {
		_log(SERVICE__ERROR, "Error in DoGetStation query: no station for id %d", sid);
		return NULL;
	}

	PyTuple * result = new PyTuple(5);
	result->items[0] = new PyInt(row.GetUInt(3));
	result->items[1] = new PyInt(row.GetUInt(2));
	result->items[2] = new PyInt(row.GetUInt(0));
	result->items[3] = new PyInt(row.GetUInt(4));
	result->items[4] = new PyInt(row.GetUInt(1));

	return result;
}

PyRep *StationDB::InstallClone( uint32 characterID, uint32 stationID, Client* who )
{
	// First fetch the clone type
	DBQueryResult res;
	DBerror err;
	uint32 itemID = 0;

	if( !sDatabase.RunQuery( res,
		"SELECT itemID"
		" FROM entity"
		" WHERE flag=400"
		" AND custominfo='active'"
		" AND ownerID=%u", characterID ))
	{
		_log(SERVICE__ERROR, "Error in InstallCloneInStation: %s", res.error.c_str() );
		return new PyBool( false );
	}

	DBResultRow row;
	if( !res.GetRow( row ) )
	{
		_log(SERVICE__ERROR, "Error in InstallCloneInStation query: no clone for character %u", characterID );
		return new PyBool( false );
	}

	itemID = row.GetInt( 0 );

	if( !sDatabase.RunQuery( err,
		"INSERT INTO chrclones("
		"id,"
		" characterID,"
		" stationID,"
		" itemID"
		")VALUES("
		"NULL,"
		" %u,"
		" %u,"
		" %u)", characterID, stationID, itemID ))
	{
		_log(SERVICE__ERROR, "Error in InstallCloneInStation when adding info: %s", res.error.c_str() );
		return new PyBool( true );
	}

	/*NotifyOnCloneInstallation n;
	 n.cloneTypeID = itemID;

	PyTuple* t = n.Encode();
	who->SendNotification( "OnShipJumpCloneInstallationDone", "cloneTypeID", &t);*/

	return new PyBool( true );
}


PyRep *StationDB::GetCharacterClones( uint32 characterID )
{
	DBQueryResult res;

	if( !sDatabase.RunQuery( res, 
		"SELECT stationID AS locationID,"
		" itemID AS jumpCloneID"
		" FROM chrclones"
		" WHERE characterID=%u", characterID ))
	{
		_log(SERVICE__ERROR, "Can't get the clones for character %u. Error: %s", characterID, res.error.c_str() );
		return new PyNone;
	}

	return DBResultToRowset( res );
}


PyRep *StationDB::DestroyClone( uint32 characterID, uint32 cloneID, uint32 locationID, Client* who )
{
	DBerror err;

	if( !sDatabase.RunQuery( err,
		"DELETE FROM chrclones"
		" WHERE stationID=%u"
		" AND itemID=%u"
		" AND characterID=%u", locationID, cloneID, characterID ))
	{
		_log(SERVICE__ERROR, "Can't delete clone %u for character %u. Error: %s", cloneID, characterID, err.c_str() );
		return NULL;
	}

	return new PyBool( true );
}
























