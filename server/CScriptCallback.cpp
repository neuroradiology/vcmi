#include "CScriptCallback.h"
#include "../lib/Connection.h"
#include "CVCMIServer.h"
#include "CGameHandler.h"
#include "../CGameState.h"
#include "../map.h"
#include "../hch/CObjectHandler.h"
#include "../hch/CTownHandler.h"
#include "../hch/CHeroHandler.h"
#include "../lib/NetPacks.h"
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
CScriptCallback::CScriptCallback(void)
{
}

CScriptCallback::~CScriptCallback(void)
{
}
void CScriptCallback::setBlockVis(int objid, bool bv)
{
	SetObjectProperty sop(objid,2,bv);
	gh->sendAndApply(&sop);
}
void CScriptCallback::setOwner(int objid, ui8 owner)
{
	SetObjectProperty sop(objid,1,owner);
	gh->sendAndApply(&sop);
}
const CGObjectInstance* CScriptCallback::getObj(int objid)
{
	return gh->gs->map->objects[objid];
}
const CGHeroInstance* CScriptCallback::getHero(int objid)
{
	return static_cast<const CGHeroInstance*>(gh->gs->map->objects[objid]);
}
const CGTownInstance* CScriptCallback::getTown(int objid)
{
	return static_cast<const CGTownInstance*>(gh->gs->map->objects[objid]);
}
void CScriptCallback::setHoverName(int objid, MetaString* name)
{
	SetHoverName shn(objid, *name);
	gh->sendAndApply(&shn);
}
int3 CScriptCallback::getPos(CGObjectInstance * ob)
{
	return ob->pos;
}
void CScriptCallback::changePrimSkill(int ID, int which, int val)
{	
	//CGHeroInstance * hero = gh->gs->map->getHero(ID,0);
	//if (which<PRIMARY_SKILLS)
	//{
	//	hero->primSkills[which]+=val;
	//	sv->playerint[hero->getOwner()]->heroPrimarySkillChanged(hero, which, val);
	//}
	//else if (which==4)
	//{
	//	hero->exp+=val;
	//	if(hero->exp >= CGI->heroh->reqExp(hero->level+1)) //new level
	//	{
	//		hero->level++;
	//		std::cout << hero->name <<" got level "<<hero->level<<std::endl;
	//		int r = rand()%100, pom=0, x=0;
	//		int std::pair<int,int>::*g  =  (hero->level>9) ? (&std::pair<int,int>::second) : (&std::pair<int,int>::first);
	//		for(;x<PRIMARY_SKILLS;x++)
	//		{
	//			pom += hero->type->heroClass->primChance[x].*g;
	//			if(r<pom)
	//				break;
	//		}
	//		std::cout << "Bohater dostaje umiejetnosc pierwszorzedna " << x << " (wynik losowania "<<r<<")"<<std::endl; 
	//		hero->primSkills[x]++;

	//		//TODO: dac dwie umiejetnosci 2-rzedne to wyboru

	//	}
	//	//TODO - powiadomic interfejsy, sprawdzic czy nie ma awansu itp
	//}
}

int CScriptCallback::getHeroOwner(int heroID)
{
	//CGHeroInstance * hero = CGI->state->map->getHero(heroID,0);
	//return hero->getOwner();
	return -1;
}
void CScriptCallback::showInfoDialog(int player, std::string text, std::vector<SComponent*> * components)
{
	//TODO: upewniac sie ze mozemy to zrzutowac (przy customowych interfejsach cos moze sie kopnac)
	//if (player>=0)
	//{
	//	CGameInterface * temp = sv->playerint[player];
	//	if (temp->human)
	//		((CPlayerInterface*)(temp))->showInfoDialog(text,*components);
	//	return;
	//}
	//else
	//{
	//	typedef std::pair<const ui8, CGameInterface*> intf;
	//	BOOST_FOREACH(intf & i, sv->playerint)
	//	{
	//		if (i.second->human)
	//			((CPlayerInterface*)(i.second))->showInfoDialog(text,*components);
	//	}
	//}
}

void CScriptCallback::showSelDialog(int player, std::string text, std::vector<CSelectableComponent*>*components, IChosen * asker)
{
	//CGameInterface * temp = sv->playerint[player];
	//if (temp->human)
	//	((CPlayerInterface*)(temp))->showSelDialog(text,*components,(int)asker);
	return;
}
int CScriptCallback::getSelectedHero()
{	
	int ret;
	//if (LOCPLINT->adventureInt->selection.type == HEROI_TYPE)
	//	ret = ((CGHeroInstance*)(LOCPLINT->adventureInt->selection.selected))->subID;
	//else 
	//	ret = -1;;
	return ret;
}
int CScriptCallback::getDate(int mode)
{
	int temp;
	switch (mode)
	{
	case 0:
		return gh->gs->day;
		break;
	case 1:
		temp = (gh->gs->day)%7;
		if (temp)
			return temp;
		else return 7;
		break;
	case 2:
		temp = ((gh->gs->day-1)/7)+1;
		if (!(temp%4))
			return 4;
		else 
			return (temp%4);
		break;
	case 3:
		return ((gh->gs->day-1)/28)+1;
		break;
	}
	return 0;
}
void CScriptCallback::giveResource(int player, int which, int val)
{
	//gh->gs->players[player].resources[which]+=val;
	//sv->playerint[player]->receivedResource(which,val);
}
void CScriptCallback::showCompInfo(int player, SComponent * comp)
{
	//CPlayerInterface * i = dynamic_cast<CPlayerInterface*>(sv->playerint[player]);
	//if(i)
	//	i->showComp(*comp);
}
void CScriptCallback::heroVisitCastle(int obj, int heroID)
{
	//CGTownInstance * n;
	//if(n = dynamic_cast<CGTownInstance*>(ob))
	//{
	//	n->visitingHero = CGI->state->map->getHero(heroID,0);
	//	gh->gs->map->getHero(heroID,0)->visitedTown = n;
	//	sv->playerint[getHeroOwner(heroID)]->heroVisitsTown(CGI->state->map->getHero(heroID,0),n);
	//}
	//else
	//	return;
}

void CScriptCallback::stopHeroVisitCastle(int obj, int heroID)
{
	//CGTownInstance * n;
	//if(n = dynamic_cast<CGTownInstance*>(ob))
	//{
	//	CGI->state->map->getHero(heroID,0)->visitedTown = NULL;
	//	if(n->visitingHero && n->visitingHero->type->ID == heroID)
	//		n->visitingHero = NULL;
	//	return;
	//}
	//else
	//	return;
}
void CScriptCallback::giveHeroArtifact(int artid, int hid, int position) //pos==-1 - first free slot in backpack
{
	CGHeroInstance* h = gh->gs->map->getHero(hid,0);
	if(position<0)
	{
		for(unsigned i=0;i<h->artifacts.size();i++)
		{
			if(!h->artifacts[i])
			{
				h->artifacts[i] = artid;
				return;
			}
		}
		h->artifacts.push_back(artid);
		return;
	}
	else
	{
		if(h->artifWorn[position]) //slot is occupied
		{
			giveHeroArtifact(h->artifWorn[position],hid,-1);
		}
		h->artifWorn[position] = artid;
	}
}

void CScriptCallback::startBattle(const CCreatureSet * army1, const CCreatureSet * army2, int3 tile, const CGHeroInstance *hero1, const CGHeroInstance *hero2) //use hero=NULL for no hero
{
	//gh->gs->battle(army1,army2,tile,hero1,hero2);
}
void CScriptCallback::startBattle(int heroID, CCreatureSet * army, int3 tile) //for hero<=>neutral army
{
	//CGHeroInstance* h = gh->gs->map->getHero(heroID,0);
	//gh->gs->battle(&h->army,army,tile,h,NULL);
}
void CLuaCallback::registerFuncs(lua_State * L)
{
//	lua_newtable(L);
//
//#define REGISTER_C_FUNC(x) \
//	lua_pushstring(L, #x);      \
//	lua_pushcfunction(L, x);    \
//	lua_rawset(L, -3)
//
//	REGISTER_C_FUNC(getPos);
//	REGISTER_C_FUNC(changePrimSkill);
//	REGISTER_C_FUNC(getGnrlText);
//	REGISTER_C_FUNC(getSelectedHero);
//
//	lua_setglobal(L, "vcmi");
//	#undef REGISTER_C_FUNC
}
int CLuaCallback::getPos(lua_State * L)//(CGObjectInstance * object);
{	
	//const int args = lua_gettop(L); // number of arguments
	//if ((args < 1) || !lua_isnumber(L, 1) )
	//	luaL_error(L,
	//		"Incorrect arguments to getPos([Object address])");
	//CGObjectInstance * object = (CGObjectInstance *)(lua_tointeger(L, 1));
	//lua_pushinteger(L,object->pos.x);
	//lua_pushinteger(L,object->pos.y);
	//lua_pushinteger(L,object->pos.z);
	return 3;
}
int CLuaCallback::changePrimSkill(lua_State * L)//(int ID, int which, int val);
{	
	//const int args = lua_gettop(L); // number of arguments
	//if ((args < 1) || !lua_isnumber(L, 1) ||
	//    ((args >= 2) && !lua_isnumber(L, 2)) ||
	//    ((args >= 3) && !lua_isnumber(L, 3))		)
	//{
	//	luaL_error(L,
	//		"Incorrect arguments to changePrimSkill([Hero ID], [Which Primary skill], [Change by])");
	//}
	//int ID = lua_tointeger(L, 1),
	//	which = lua_tointeger(L, 2),
	//	val = lua_tointeger(L, 3);

	//CScriptCallback::changePrimSkill(ID,which,val);

	return 0;
}
int CLuaCallback::getGnrlText(lua_State * L) //(int which),returns string
{
	//const int args = lua_gettop(L); // number of arguments
	//if ((args < 1) || !lua_isnumber(L, 1) )
	//	luaL_error(L,
	//		"Incorrect arguments to getGnrlText([Text ID])");
	//int which = lua_tointeger(L,1);
	//lua_pushstring(L,CGI->generaltexth->allTexts[which].c_str());
	return 1;
}
int CLuaCallback::getSelectedHero(lua_State * L) //(),returns int (ID of hero, -1 if no hero is seleceted)
{
	//int ret;
	//if (LOCPLINT->adventureInt->selection.type == HEROI_TYPE)
	//	ret = ((CGHeroInstance*)(LOCPLINT->adventureInt->selection.selected))->subID;
	//else 
	//	ret = -1;
	//lua_pushinteger(L,ret);
	return 1;
}
