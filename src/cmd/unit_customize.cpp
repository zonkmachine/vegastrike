#include "unit.h"
#include "unit_factory.h"
#include "images.h"
#include "universe.h"
#include "vegastrike.h"
#include <stdio.h>
#include <stdlib.h>
#include "gfx/cockpit.h"
#include "savegame.h"
#include "config_xml.h"
#include "xml_serializer.h"
#include "audiolib.h"
#include "vs_globals.h"
#include "unit_const_cache.h"
#ifdef _WIN32
#define strcasecmp stricmp
#endif
extern int GetModeFromName (const char *);

void GameUnit::GameMount::ReplaceMounts (const Unit::Mount *other) {
  Mount::ReplaceMounts( other);
  sound = AUDCreateSound (sound,type->type!=weapon_info::PROJECTILE);//copy constructor basically
}

Unit * CreateGameTurret (std::string tur,int faction) {
  return UnitFactory::createUnit (tur.c_str(),true,faction);
}
bool GameUnit::UpgradeSubUnits (const Unit * up, int subunitoffset, bool touchme, bool downgrade, int &numave, double &percentage)  {
  bool bl = UpgradeSubUnitsWithFactory( up, subunitoffset, touchme, downgrade, numave, percentage,&CreateGameTurret);
  DisableTurretAI();
  return bl;
}
extern char * GetUnitDir (const char *);
double GameUnit::Upgrade (const std::string &file, int mountoffset, int subunitoffset, bool force, bool loop_through_mounts) {
#if 0
  if (shield.number==2) {
    printf ("shields before %s %f %f",file.c_str(),shield.fb[2],shield.fb[3]);
  }else {
    printf ("shields before %s %d %d",file.c_str(),shield.fbrl.frontmax,shield.fbrl.backmax);    

  }
#endif
  const Unit * up = getCachedConstUnit (file,FactionUtil::GetFaction("upgrades"));
  if (!up) {
    up = setCachedConstUnit (file,
			     FactionUtil::GetFaction("upgrades"),
			     UnitFactory::createUnit (file.c_str(),true,FactionUtil::GetFaction("upgrades")));
  }
  char * unitdir  = GetUnitDir(name.c_str());
  string templnam = string(unitdir)+".template";	  
  const Unit * templ = getCachedConstUnit (templnam,faction);
	if (templ==NULL) {
	  templ = setCachedConstUnit (templnam,faction,UnitFactory::createUnit (templnam.c_str(),true,this->faction));
	}
	free (unitdir);
	double percentage=0;
	if (up->name!="LOAD_FAILED") {
	  
	  for  (int i=0;percentage==0;i++ ) {
		  if (!this->Unit::Upgrade(up,mountoffset+i, subunitoffset+i, GetModeFromName(file.c_str()),force, percentage,(templ->name=="LOAD_FAILED")?NULL:templ)) {
	      percentage=0;
	    }
	    if (!loop_through_mounts||(i+1>=this->GetNumMounts ())) {
	      break;
	    }
	  }
	}
#if 0
  if (shield.number==2) {
    printf ("shields before %s %f %f",file.c_str(),shield.fb[2],shield.fb[3]);
  }else {
    printf ("shields before %s %d %d",file.c_str(),shield.fbrl.frontmax,shield.fbrl.backmax);    

  }
#endif

	return percentage;
}
