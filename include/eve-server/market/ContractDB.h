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
	Author:		Almamu
*/

#ifndef __CONTRACTDB_H_INCL__
#define __CONTRACTDB_H_INCL__

#include "ServiceDB.h"

class PyRep;

typedef enum{
	conStatusExpired = 8,
	conStatusFailed = 7,
	conStatusRejected = 6,
	conStatusCancelled = 5,
	conStatusFinished = 4,
	conStatusFinishedContractor = 3,
	conStatusFinishedIssuer = 2,
	conStatusInProgress = 1,
	conStatusOutstanding = 0
}contractStatus;

class ContractDB
{
public:
	bool LoadContracts( std::map<uint32, Contract*> &into );
	bool SaveContract( Contract* contract );
	bool GetContractItems( uint32 contractID, std::map<uint32, uint32>& into );
	Contract* GetContractInfo( uint32 contractID );
protected:
	
};





#endif


