// CMT.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <boost/thread.hpp>
#include "SDL_TTF.h"
#include "SDL_mixer.h"
#include "SDL_Extensions.h"
#include "SDL_framerate.h"
#include "CGameInfo.h"
#include "mapHandler.h"
#include "global.h"
#include "CPreGame.h"
#include "CConsoleHandler.h"
#include "CCursorHandler.h"
#include "CScreenHandler.h"
#include "CPathfinder.h"
#include "CGameState.h"
#include "CCallback.h"
#include "CPlayerInterface.h"
#include "CLuaHandler.h"
#include "CLua.h"
#include "CAdvmapInterface.h"
#include "hch/CBuildingHandler.h"
#include "hch/CVideoHandler.h"
#include "hch/CAbilityHandler.h"
#include "hch/CHeroHandler.h"
#include "hch/CCreatureHandler.h"
#include "hch/CSpellHandler.h"
#include "hch/CBuildingHandler.h"
#include "hch/CMusicHandler.h"
#include "hch/CLodHandler.h"
#include "hch/CDefHandler.h"
#include "hch/CAmbarCendamo.h"
#include "hch/CGeneralTextHandler.h"
#include "client/Graphics.h"
#include "Client/Client.h"
#include "lib/Connection.h"
#include "lib/VCMI_Lib.h"
std::string NAME = NAME_VER + std::string(" (client)");
DLL_EXPORT void initDLL(CLodHandler *b);
SDL_Surface * screen, * screen2;
extern SDL_Surface * CSDL_Ext::std32bppSurface;

std::queue<SDL_Event> events;
boost::mutex eventsM;

TTF_Font * TNRB16, *TNR, *GEOR13, *GEORXX, *GEORM, *GEOR16;

int _tmain(int argc, _TCHAR* argv[])
{ 
	std::cout.flags(ios::unitbuf);
	std::cout << NAME << std::endl;
	srand ( time(NULL) );
	CPG=NULL;
	atexit(SDL_Quit);
	CGameInfo * cgi = CGI = new CGameInfo; //contains all global informations about game (texts, lodHandlers, map handler itp.)
	//CLuaHandler luatest;
	//luatest.test(); 
		//CBIKHandler cb;
		//cb.open("CSECRET.BIK");
	std::cout << "Starting... " << std::endl;
	THC timeHandler tmh, total, pomtime;
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO/*|SDL_INIT_EVENTTHREAD*/)==0)
	{
		screen = SDL_SetVideoMode(800,600,24,SDL_SWSURFACE|SDL_DOUBLEBUF/*|SDL_FULLSCREEN*/);  //initializing important global surface
		THC std::cout<<"\tInitializing screen: "<<pomtime.getDif()<<std::endl;
		SDL_WM_SetCaption(NAME.c_str(),""); //set window title
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			int rmask = 0xff000000;int gmask = 0x00ff0000;int bmask = 0x0000ff00;int amask = 0x000000ff;
		#else
			int rmask = 0x000000ff;	int gmask = 0x0000ff00;	int bmask = 0x00ff0000;	int amask = 0xff000000;
		#endif
		CSDL_Ext::std32bppSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, 1, 1, 32, rmask, gmask, bmask, amask);
		THC std::cout<<"\tInitializing minors: "<<pomtime.getDif()<<std::endl;
		TTF_Init();
		TNRB16 = TTF_OpenFont("Fonts\\tnrb.ttf",16);
		GEOR13 = TTF_OpenFont("Fonts\\georgia.ttf",13);
		GEOR16 = TTF_OpenFont("Fonts\\georgia.ttf",16);
		GEORXX = TTF_OpenFont("Fonts\\tnrb.ttf",22);
		GEORM = TTF_OpenFont("Fonts\\georgia.ttf",10);
		atexit(TTF_Quit);
		THC std::cout<<"\tInitializing fonts: "<<pomtime.getDif()<<std::endl;
		CMusicHandler * mush = new CMusicHandler;  //initializing audio
		mush->initMusics();
		//audio initialized 
		cgi->consoleh = new CConsoleHandler;
		cgi->mush = mush;
		cgi->curh = new CCursorHandler; 
		THC std::cout<<"\tInitializing sound and cursor: "<<pomtime.getDif()<<std::endl;
		THC std::cout<<"Initializing screen, fonts and sound handling: "<<tmh.getDif()<<std::endl;
		CDefHandler::Spriteh = cgi->spriteh = new CLodHandler();
		cgi->spriteh->init("Data\\H3sprite.lod","Sprites");
		BitmapHandler::bitmaph = cgi->bitmaph = new CLodHandler;
		cgi->bitmaph->init("Data\\H3bitmap.lod","Data");
		THC std::cout<<"Loading .lod files: "<<tmh.getDif()<<std::endl;
		initDLL(cgi->bitmaph);

		CGI->arth = VLC->arth;
		CGI->creh = VLC->creh;
		CGI->townh = VLC->townh;
		CGI->heroh = VLC->heroh;
		CGI->objh = VLC->objh;
		CGI->dobjinfo = VLC->dobjinfo;

		THC std::cout<<"Initializing VCMI_Lib: "<<tmh.getDif()<<std::endl;

		//cgi->curh->initCursor();
		//cgi->curh->showGraphicCursor();
		pomtime.getDif();
		cgi->screenh = new CScreenHandler;
		cgi->screenh->initScreen();
		THC std::cout<<"\tScreen handler: "<<pomtime.getDif()<<std::endl;

		CAbilityHandler * abilh = new CAbilityHandler;
		abilh->loadAbilities();
		cgi->abilh = abilh;
		THC std::cout<<"\tAbility handler: "<<pomtime.getDif()<<std::endl;

		THC std::cout<<"Preparing first handlers: "<<tmh.getDif()<<std::endl;
		pomtime.getDif();
		graphics = new Graphics();
		THC std::cout<<"\tMain graphics: "<<tmh.getDif()<<std::endl;
		std::vector<CDefHandler **> animacje;
		for(std::vector<CHeroClass *>::iterator i = cgi->heroh->heroClasses.begin();i!=cgi->heroh->heroClasses.end();i++)
			animacje.push_back(&((*i)->*(&CHeroClass::moveAnim)));
		graphics->loadHeroAnim(animacje);
		THC std::cout<<"\tHero animations: "<<tmh.getDif()<<std::endl;
		THC std::cout<<"Initializing game graphics: "<<tmh.getDif()<<std::endl;
		CMessage::init();
		cgi->generaltexth = new CGeneralTextHandler;
		cgi->generaltexth->load();
		THC std::cout<<"Preparing more handlers: "<<tmh.getDif()<<std::endl;
		CPreGame * cpg = new CPreGame(); //main menu and submenus
		THC std::cout<<"Initialization CPreGame (together): "<<tmh.getDif()<<std::endl;
		THC std::cout<<"Initialization of VCMI (togeter): "<<total.getDif()<<std::endl;
		cpg->mush = mush;
		StartInfo *options = new StartInfo(cpg->runLoop());
///////////////////////////////////////////////////////////////////////////////////////

		boost::thread servthr(boost::bind(system,"VCMI_server.exe > server_log.txt")); //runs server executable; 
												//TODO: will it work on non-windows platforms?
		THC tmh.getDif();pomtime.getDif();//reset timers


		CSpellHandler * spellh = new CSpellHandler;
		spellh->loadSpells();
		cgi->spellh = spellh;		
		THC std::cout<<"\tSpell handler: "<<pomtime.getDif()<<std::endl;

		CBuildingHandler * buildh = new CBuildingHandler;
		buildh->loadBuildings();
		cgi->buildh = buildh;
		THC std::cout<<"\tBuilding handler: "<<pomtime.getDif()<<std::endl;



		cgi->pathf = new CPathfinder();
		THC std::cout<<"\tPathfinder: "<<pomtime.getDif()<<std::endl;
		cgi->consoleh->runConsole();
		THC std::cout<<"\tCallback and console: "<<pomtime.getDif()<<std::endl;
		THC std::cout<<"Handlers initialization (together): "<<tmh.getDif()<<std::endl;

		std::ofstream lll("client_log.txt");
		CConnection c("localhost","3030",NAME,lll);
		THC std::cout<<"\tConnecting to the server: "<<tmh.getDif()<<std::endl;

		CClient cl(&c,options);
		boost::thread t(boost::bind(&CClient::run,&cl));

		SDL_Event ev;
		while(1) //main SDL events loop
		{
			SDL_WaitEvent(&ev);
			if(ev.type==SDL_QUIT) 
			{
				t.interrupt();
				exit(0);
			}
			eventsM.lock();
			events.push(ev);
			eventsM.unlock();
		}

		///claculating FoWs for minimap
		/****************************Minimaps' FoW******************************************/
		//for(int g=0; g<cgi->playerint.size(); ++g)
		//{
		//	if(!cgi->playerint[g]->human)
		//		continue;
		//	CMinimap & mm = ((CPlayerInterface*)cgi->playerint[g])->adventureInt->minimap;

		//	int mw = mm.map[0]->w, mh = mm.map[0]->h,
		//		wo = mw/CGI->mh->sizes.x, ho = mh/CGI->mh->sizes.y;

		//	for(int d=0; d<cgi->mh->map->twoLevel+1; ++d)
		//	{
		//		SDL_Surface * pt = CSDL_Ext::newSurface(mm.pos.w, mm.pos.h, CSDL_Ext::std32bppSurface);

		//		for (int i=0; i<mw; i++)
		//		{
		//			for (int j=0; j<mh; j++)
		//			{
		//				int3 pp( ((i*CGI->mh->sizes.x)/mw), ((j*CGI->mh->sizes.y)/mh), d );

		//				if ( !((CPlayerInterface*)cgi->playerint[g])->cb->isVisible(pp) )
		//				{
		//					CSDL_Ext::SDL_PutPixelWithoutRefresh(pt,i,j,0,0,0);
		//				}
		//			}
		//		}
		//		CSDL_Ext::update(pt);
		//		mm.FoW.push_back(pt);
		//	}

		//}
	}
	else
	{
		printf("Something was wrong: %s/n", SDL_GetError());
		return -1;
	}
}
