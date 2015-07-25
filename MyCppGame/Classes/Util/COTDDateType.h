//
//  COTDDateType.h
//
//  Created by Javier Fuchs on 07/25/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//
//

#ifndef __COTD_DATE_TYPE_H__
#define __COTD_DATE_TYPE_H__

#include <iostream>
#include <iomanip>
#include <strstream>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>


#ifdef OTL_ODBC_MYSQL
#include <iostream>
#include "otlv4.h" // include the OTL 4 header file
#endif




class COTDDateType {
private:
  bool _null;
  bool _sysdate;
public:

  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int minute;
  unsigned int second;

  COTDDateType() { setNull(); }

  const bool isNotNull() const { return (_null == false); }
  const bool isNull() const { return (_null == true); }
  const bool isSysdate() const { return (_sysdate == true); }
  void setNull() { 
      _null = true; 
      _sysdate = false; 
      year = month = day = hour = minute = second = 0;
  }
  void setNoNull() { _null = false; }
  void setSysdate() { setNoNull(); _sysdate = true; }

  friend std::ostream& operator << ( std::ostream& os, const COTDDateType& d ) { 
      if (d.isNull()) {
	  os << "NULL";
      } else if (d.isSysdate()) {
	  os << "SYSDATE";
      } else {
	  os << d.year
	     << std::setfill('0') << setiosflags(std::ios::right) << std::setw(2) 
	     << d.month
	     << std::setfill('0') << setiosflags(std::ios::right) << std::setw(2) 
	     << d.day
	     << std::setfill('0') << setiosflags(std::ios::right) << std::setw(2) 
	     << d.hour
	     << std::setfill('0') << setiosflags(std::ios::right) << std::setw(2) 
	     << d.minute
	     << std::setfill('0') << setiosflags(std::ios::right) << std::setw(2) 
	     << d.second
	     << std::setfill(' ');
      }
      return os; 
  }

  COTDDateType& operator=(const time_t& rt) {
      if (rt <= 0) {
	  setNull();
      }
      setNoNull();
      struct tm *ptm = gmtime(&rt);
      year = ptm->tm_year + 1900;
      month = ptm->tm_mon + 1;
      day = ptm->tm_mday;
      hour = ptm->tm_hour;
      minute = ptm->tm_min;
      second = ptm->tm_sec;
      return (*this);
   }

   COTDDateType& operator=(COTDDate& sd) {
       setNoNull();
       time_t gmt = sd.toTime();
       struct tm *ptm = gmtime(&gmt);
       year = ptm->tm_year + 1900;
       month = ptm->tm_mon + 1;
       day = ptm->tm_mday;
       hour = ptm->tm_hour;
       minute = ptm->tm_min;
       second = ptm->tm_sec;
       return (*this);
  }


  bool operator == (const COTDDateType& that) const {
      return (toTime() == that.toTime());
  }

  bool operator == (COTDDate& sd) const {
      return (toTime() == sd.toTime());
  }

  bool operator == (time_t t) const {
      return (toTime() == t);
  }

  bool operator != (time_t t) const {
      return (toTime() != t);
  }

  bool operator < (COTDDate& sd) const {
      return (toTime() < sd.toTime());
  }

  bool operator <= (COTDDate& sd) const {
      return (toTime() <= sd.toTime());
  }

  bool operator >= (COTDDate& sd) const {
      return (toTime() >= sd.toTime());
  }

  bool operator > (COTDDate& sd) const {
      return (toTime() > sd.toTime());
  }

  time_t toTime() const 
  {
     if (isNull()) {
	 return ((time_t)-1);
     } 
     struct tm tm0;
     tm0.tm_year = year - 1900;
     tm0.tm_mon = month - 1;
     tm0.tm_mday = day;
     tm0.tm_hour = hour;
     tm0.tm_min = minute;
     tm0.tm_sec = second;
     tm0.tm_isdst = -1;
     return (mktime(&tm0));
  }

  time_t toGmtTime(int timezone) const 
  {
     if (isNull()) {
	 return ((time_t)-1);
     } 
     struct tm tm0;
     tm0.tm_year = year - 1900;
     tm0.tm_mon = month - 1;
     tm0.tm_mday = day;
     tm0.tm_hour = hour;
     tm0.tm_min = minute;
     tm0.tm_sec = second;
     tm0.tm_isdst = -1;
     return (mktime(&tm0) + (timezone*3600));
  }

#ifdef OTL_ODBC_MYSQL

  void toOtl(otl_value<otl_datetime>& otlDate) const {
      if (isNull()) {
	  otlDate.set_null();
      } else {
	  otlDate.set_non_null();
	  otlDate.v.year = year;
	  otlDate.v.month = month;
	  otlDate.v.day = day;
	  otlDate.v.hour = hour;
	  otlDate.v.minute = minute;
	  otlDate.v.second = second;
      }
  }

  COTDDateType& operator=(const otl_value<otl_datetime>& otlDate) {

      if (otlDate.is_null()) {
	  setNull();
      } else {
	  setNoNull();
	  year = otlDate.v.year;
	  month = otlDate.v.month;
	  day = otlDate.v.day;
	  hour = otlDate.v.hour;
	  minute = otlDate.v.minute;
	  second = otlDate.v.second;
      }
      return (*this);
  }
#endif

};


#endif // __COTD_DATE_TYPE_H__
