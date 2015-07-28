//
//  COTDLog.cpp
//
//  Created by Javier Fuchs on 07/25/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//
#include <strstream>

#ifdef _COTDLOG_TEST_
#define S_LOG
#endif
#ifndef _ALLOW_STD_TYPES_
#define _ALLOW_STD_TYPES_
#endif
#include "COTDLog.h"

/**
 * Constructor por default
 */
COTDLog::COTDLog()
{
   vater = 0;
   setpid(::getpid());
   stdOutput = true;
   logLevel = thisLine = oldLine = LOGQUIET;
   open(getName());
}

char* COTDLog::getName() {
   std::strstream Nombre;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    cocos2d::FileUtils* fu = cocos2d::FileUtils::getInstance();
    Nombre << fu->getWritablePath();
    COTDDate d;
    Nombre << "COTD_"
    << (getenv("USERNAME") ?: "No_USERNME") << '_'	// obtiene usuario linux que hizo login al equipo
    << d << ".log";
    Nombre << '\0';
    return Nombre.str();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_LINUX
    if (getenv("COTD_LOG_DIR") == NULL) {
        std::cerr << "variable de ambiente COTD_LOG_DIR no existe" << nl;
        Nombre << "/tmp";
    } else {
        Nombre << getenv("COTD_LOG_DIR");
    }
    Nombre << "/";
    if (getenv("COTD_LOG_FILE") == NULL) {
        std::cerr << "variable de ambiente COTD_LOG_FILE no existe" << nl;
        COTDDate d;
        //       std::string username = getenv("USERNAME");
        Nombre << "COTD_"
        << (getenv("USERNAME") ?: "No_USERNME") << '_'	// obtiene usuario linux que hizo login al equipo
        << d << ".log";
    } else {
        Nombre << getenv("COTD_LOG_FILE");
    }
    Nombre << '\0';
    return Nombre.str();
#else
    return "COTDLog";
#endif
}

/**
 * Constructor
 * @param name
 */
COTDLog::COTDLog(const char* name)
{
   vater = 0;
   setpid(::getpid());
   logLevel = thisLine = oldLine = LOGQUIET;
   stdOutput = false;
   open(name);
}

/**
 * Desctructor
 */
COTDLog::~COTDLog()
{
    COTDDate dtLog;
	if (vater == 0) {
		fs << nl << nl << nl
		   << "--- " << getpid() 
		   << ' ' << __FILE__ << ' ' << __FUNCTION__ << "()" << ':' << __LINE__ << ' ' << nl
		   << std::setfill('=') << std::setiosflags(std::ios::left) << std::setw(80) << '=' << nl
		   << "PID: " << getpid() << " - END Sesion: " << dtLog << nl
		   << ' ' << __FILE__ << ' ' << __FUNCTION__ << "()" << ':' << __LINE__ << ' ' << nl
		   << std::setfill('=') << std::setiosflags(std::ios::left) << std::setw(80) << '=' << nl;
	} else {
		fs << nl << nl << nl
		   << "--- " << getpid() << "END child session " << getpid() << "." << nl;
	}
	// WARNING!!
	// El hijo cierra el log cuando se muere. Seria mejor que no lo hiciera. 
	close();
}

/**
 * Abrir archivo
 */
void COTDLog::open() {
	fs << nl << nl << nl
	   << "--- " << getpid() << "OPEN log" << nl;
	open(getName());
}

void COTDLog::close() {
	fs << nl << nl << nl
	   << "--- " << getpid() << "CLOSE log" << nl;
	fs.close();
}

/**
 * Abrir archivo
 * @param name
 */
void COTDLog::open(const char* name)
{
   // reopening a stream fails  
   fs.clear(); // este clear es para este bug en la libstdc++
   // Ver: http://gcc.gnu.org/onlinedocs/libstdc++/faq/index.html#4_0

   if (name == NULL) {
       std::cerr << "no esta seteado el nombre del archivo" << nl;
       setStdOutput();
       return;
   }
   fs.open(name, std::ios::out | std::ios::app);

   if (!fs) {
       std::cerr << "cannot open: " << name << nl;
   }
   COTDDate dtLog;
   fs << "--- " << getpid() 
      << ' ' << __FILE__ << ' ' << __FUNCTION__ << "()" << ':' << __LINE__ << ' ' << nl
      << std::setfill('=') << std::setiosflags(std::ios::left) << std::setw(80) << '=' << nl
      << "PID: " << getpid() << " - INICIO Sesion: " << dtLog << nl
      << std::setfill('=') << std::setiosflags(std::ios::left) << std::setw(80) << '=' << nl;
   fs.flush();
}

#ifdef _COTDLOG_TEST_
/**
 * Test
 * 
 * @return int
 */

// g++  -Wno-deprecated -I./ -DLINUX -D_COTDLOG_TEST_ COTDLog.cpp -o COTDLogTest

int f() {
    dbg << endl; 
    dbg << "HI" << endl; 
    for (long i = 0; i < 2000000; i++);
    return 99;
}
int main()
{
    slog.setStdOutput();
    slog.setOn();
    COTDDate d;
    inf << "Test de la clase COTDLog()" << nl
        << "Fecha: " << d << endl;
    inf << "int ";
    slog << 10;   


    slog << "float ";
    slog << 10.0f;
    slog << endl;
    slog << getpid() << ": log creado." << endl;
    std::cout << getpid() << ": log creado." << endl;


    inf << " ---> INF0 <--- " << f() << endl;
    dbg << " ---> DBG <--- " << f() << endl;
    inf << " ---> INF1 <--- " << f() << endl;
    err << " ---> ERR <--- " << f() << endl;
    inf << " ---> INF2 <--- " << f() << endl;
    wrn << " ---> WRN <--- " << f() << endl;
    inf << " ---> INF3 <--- " << f() << endl;

	slog.close();
	inf << "Cerro" << endl;
	slog.open();
    slog.setOn();
	inf << "Abrio" << endl;
	inf << "FIN TEST" << endl;

    return 0;
}



#endif
