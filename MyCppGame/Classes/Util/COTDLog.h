//
//  COTDLog.h
//
//  Created by Javier Fuchs on 07/25/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//

#ifndef __COTDLOG_H__
#define __COTDLOG_H__

#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <fstream>
#include "COTDDate.h"

#define nl '\n' 
#define endl nl , (slog.setThisLine(slog.getOldLine()))

class COTDLog {
protected:
   std::fstream  fs;
   int logLevel; // Log level de la instancia de log
   int thisLine; // Log level de la linea que se va a imprimir
   int oldLine; // Log level de la linea padre
   bool stdOutput;
   pid_t pid;
   pid_t vater;
   std::string datetime;

private:
   char* getName();
   
protected:
public:
   COTDLog();
   COTDLog(const char* name);
   ~COTDLog();

   static const int LOGQUIET = 0x0;	// no imprime mucho :)
   static const int LOGERROR = 0x1;	// solo errores
   static const int LOGWARNING = 0x2;	// errores y warnings
   static const int LOGINFO  = 0x3;	// errores, warnings e info
   static const int LOGDEBUG = 0x4;	// errores, warnings, info y debug
   static const int LOGFULL  = LOGDEBUG;

   bool isOn() { return (logLevel != LOGQUIET); }
   bool isOff() { return (logLevel == LOGQUIET); }

   void showLogLevels(std::string& showLevels) { 
       std::strstream aux;
       aux << LOGQUIET << " = LOGQUIET" << ", "
           << LOGERROR << " = LOGERROR" << ", "
           << LOGWARNING << " = LOGWARNING" << ", "
           << LOGINFO << " = LOGINFO" << ", "
           << LOGDEBUG << " = LOGDEBUG" << ", "
           << LOGFULL << " = LOGFULL" 
           << '\0';
       showLevels = aux.str();
   }

   void setStdOutput() { stdOutput = true; }
   bool getStdOutput() { return stdOutput; }
   int getThisLine() { return thisLine; }
   int getOldLine() { return oldLine; }
   void setThisLine(int value) { thisLine = value; }
   void setOldLine(int value) { oldLine = value; }

   void setOn()  { 
       logLevel = LOGFULL; 
       std::string desc;
       fs << "Seteo Debug ON" << nl
          << "Level:"  << (*this)(desc);
       fs << " - " << desc << nl;
   }
   void setOff() { 
       logLevel = LOGQUIET; 
       std::string desc;
       fs << "Seteo Debug OFF (QUIET)" << nl
          << "Level:"  << (*this)(desc);
       fs << " - " << desc << nl;
   }

   // Seteos que indican que nivel de log esta imprimiendo
   void setLineError()   { setOldLine(thisLine); setThisLine(LOGERROR); }
   void setLineWarning() { setOldLine(thisLine); setThisLine(LOGWARNING); }
   void setLineInfo()    { setOldLine(thisLine); setThisLine(LOGINFO); }
   void setLineDebug()   { setOldLine(thisLine); setThisLine(LOGDEBUG); }

   void open();
   void close();
   void open(const char* name);

   /**
    * Operador de salida
    * @param kind cualquier tipo de dato
    * @param sl una instancia
    * @param p el dato
    * 
    * @return COTDLog&
    */

   template <class kind>
   friend COTDLog& operator << (COTDLog& sl, kind p)
   {
      if(sl.isOn())
      {
		  if (sl.getThisLine() <= sl.logLevel) {
			  sl.fs << p;
			  sl.fs.flush();
			  if (sl.getStdOutput()) {
				  std::cout << p;
			  }
		  }
      } else {
		  if (sl.getStdOutput()) {
			  std::cout << p;
		  }
      }

      return (sl);
   }

   void operator [](int newLogLevel) {
       if (newLogLevel >= LOGQUIET && newLogLevel <= LOGFULL)
	   logLevel = newLogLevel; 
   }

   const int operator ()(std::string& desc)
   {
       if (logLevel == LOGQUIET) {
	   desc = "LOGQUIET";
       }
       if (logLevel == LOGERROR) {
	   desc = "LOGERROR";
       }
       if (logLevel == LOGWARNING) {
	   desc = "LOGWARNING";
       }
       if (logLevel == LOGINFO) {
	   desc = "LOGINFO";
       }
       if (logLevel == LOGDEBUG) {
	   desc = "LOGDEBUG";
       }
       return logLevel;
   }
   // prefix --r
   COTDLog& operator --(void)
   {
       if (logLevel > LOGQUIET)
	   logLevel--;
       return (*this);
   }
   // prefix ++r
   COTDLog& operator ++(void)
   {
       if (logLevel < LOGFULL)
	   logLevel++;
       return (*this);
   }

   void setpid(pid_t newpid) {
	   vater = pid;
       pid = newpid;
   }

   pid_t getpid() const {
       return pid;
   }

   const char* getLocalTime() {
      COTDDate d((char *)NULL);
      datetime = d.Str;
      return datetime.c_str();
   }
};

#ifndef S_LOG
extern COTDLog slog;
#else
#  ifdef _COTDLOG_TEST_
   COTDLog slog("./servbrk.log");
#  else
   COTDLog slog;
#  endif
#endif

#define LOGINFORMATION ' ' << slog.getpid() << ' ' << slog.getLocalTime() << ' ' << __FILE__ << ' ' << __FUNCTION__ << "()" << ':' << __LINE__ << ' '
#define err slog.setLineError();   slog << "ERR" << LOGINFORMATION
#define wrn slog.setLineWarning(); slog << "WRN" << LOGINFORMATION
#define inf slog.setLineInfo();    slog << "INF" << LOGINFORMATION
#define dbg slog.setLineDebug();   slog << "DBG" << LOGINFORMATION


#endif // __COTDLOG_H__
