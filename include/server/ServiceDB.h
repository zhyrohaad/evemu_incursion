/*
	------------------------------------------------------------------------------------
	LICENSE:
	------------------------------------------------------------------------------------
	This file is part of EVEmu: EVE Online Server Emulator
	Copyright 2006 - 2008 The EVEmu Team
	For the latest information visit http://evemu.mmoforge.org
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


#ifndef __SERVICEDB_H_INCL__
#define __SERVICEDB_H_INCL__

#include "../common/common.h"
#include "../common/packet_types.h"

#include <string>
#include <vector>
#include <map>

/*                                                                              
 * This object is the home for common DB operations which may be needed by many
 * different service objects. It should be inherited by each service DB
 * implementation.
 *
 */

class DBcore;
class DBResultRow;
class GPoint;
class ItemFactory;

class PyRepObject;
class PyRepDict;
class EntityList;
class Client;
class CharacterData;
class CorpMemberInfo;
//class InventoryItem;

class ServiceDB {
public:
	ServiceDB(DBcore *db);
	ServiceDB(ServiceDB *db);	//alternate constructor which simply steals the dbcore from a different serviceDB object to prevent having to expose it.
	virtual ~ServiceDB();

	bool DoLogin(const char *login, const char *pass, uint32 &out_accountID, uint32 &out_role);

	//entity/item stuff:
	bool ListEntitiesByCategory(uint32 ownerID, uint32 categoryID, std::vector<uint32> &into);
	uint32 GetCurrentShipID(uint32 characterID);
	PyRepObject *GetInventory(uint32 entityID, EVEItemFlags flag);

	void SetCharacterLocation(uint32 characterID, uint32 stationID, uint32 systemID, uint32 constellationID, uint32 regionID);
	
	PyRepObject *GetSolRow(uint32 systemID) const;
	PyRepObject *GetSolDroneState(uint32 systemID) const;

	void MoveCharacter(uint32 charID);
	
	bool GetSystemInfo(uint32 systemID, uint32 *constellationID, uint32 *regionID, std::string *name, std::string *securityClass);	// mapSolarSystems
	bool GetStaticItemInfo(uint32 itemID, uint32 *systemID, uint32 *constellationID, uint32 *regionID, GPoint *position);	// mapDenormalize
	bool GetStationInfo(uint32 stationID, uint32 *systemID, uint32 *constellationID, uint32 *regionID, GPoint *position, GPoint *dockPosition, GVector *dockOrientation);	// staStations

	uint32 GetDestinationStargateID(uint32 fromSystem, uint32 toSystem);

	bool GetConstant(const char *name, uint32 &into);
	
	//these really want to move back into AccountDB
	bool GiveCash( uint32 characterID, JournalRefType refTypeID, uint32 ownerFromID, uint32 ownerToID, const char *argID1, uint32 accountID, EVEAccountKeys accountKey, double amount, double balance, const char *reason);
	bool AddBalanceToCorp(uint32 corpID, double amount);
	double GetCorpBalance(uint32 corpID);

	//johnsus - serverStartType mod
	void SetServerOnlineStatus(bool onoff_status);
	//johnsus - characterOnline mod
	void SetCharacterOnlineStatus(uint32 char_id, bool onoff_status);

	void SetAccountOnlineStatus(uint32 accountID, bool onoff_status);

protected:
	DBcore *const m_db;	//we do not own this pointer
	
	void ProcessStringChange(const char * key, const std::string & oldValue, const std::string & newValue, PyRepDict * notif, std::vector<std::string> & dbQ);
	void ProcessRealChange(const char * key, double oldValue, double newValue, PyRepDict * notif, std::vector<std::string> & dbQ);
	void ProcessIntChange(const char * key, uint32 oldValue, uint32 newValue, PyRepDict * notif, std::vector<std::string> & dbQ);

private:

	/**
	 * CreateNewAccount
	 *
	 * This method is part of the "autoAccount" creation patch by firefoxpdm. This
	 * will insert a new account row into the database if the account name doesn't
	 * exist at login.
	 *
	 * @param accountName is a const char string containing the name.
	 * @param accountPass is a const char string containing the password.
	 * @param role is the users role in the game.
	 * @author firefoxpdm, xanarox
	 */
	bool CreateNewAccount(const char * accountName, const char * accountPass, uint64 role); // autoAccount feature
};

#endif


