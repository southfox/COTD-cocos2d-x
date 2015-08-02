//
//  COTDDate.h
//
//  Created by Javier Fuchs on 07/25/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//
//

#ifndef __COTD_DATE_H__
#define __COTD_DATE_H__

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



/** COTDDate
 * Obtiene la fecha/hora/centesimas del SO en diferentes formatos
 * @author jfuchs 
 */

class COTDDate {
protected:
   // Atributos privados
   /// First Weeday of YearMin
   enum { Sunday = 0, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday } enumWeekDay;

   static const int firstWeekDay = 8;
   /// Min Year
   static const int YearMin = 1900;
   /// Max Year
   static const int YearMax = 2099;
   /// Estructura de Fecha
   struct timeval    tvDate;
   /// Centesimas: CC
   std::string          c;
   /// Date: YYYYMMDD
   std::string          d;
   /// Time: HHMISS
   std::string          t;
   /// DateTime: YYYYMMDDHHMISS
   std::string          dt;
   /// DateTimeCent: YYYYMMDDHHMISSCC
   std::string          dtc;
private:
   /**
    * Construir una fecha
    * @param pFmt
    * 
    * @return std::string
    */
   std::string build(const char *pFmt)
   {
      if (!gettimeofday(&tvDate, NULL))
         setOk();

      char szBuf[128];
      strftime(szBuf, 128, pFmt, localtime(&(tvDate.tv_sec)));
      assert(strlen(szBuf));

      // Microsegundos / 10000 = Centesimas
      std::strstream sAux;
      sAux << tvDate.tv_usec/10000 << '\0';
      c = sAux.str();

//      std::string  sBuf;
//      sBuf = szBuf;
       return szBuf;
   }

public:

   bool status;

   void setOk() { status = true; }

   /**@name constructors */
   //@{
   /// Default, obtiene la fecha/hora/Centesimas con formatos predefinidos
   COTDDate()
   {
      status = false;
      // Dia: YYYYMMDD
      d = build((char*) "%Y%m%d");
      // Hora: HHMISS
      t = build((char*) "%H%M%S");
      // Dia+Hora: YYYYMMDDHHMISS
      dt = d;
      dt += t;

      // Microsegundos / 10000 = Centesimas
      std::strstream sAux;
      sAux << std::setfill('0') << std::setiosflags(std::ios::left) << std::setw(2) << tvDate.tv_usec/10000 << '\0';
      c = sAux.str();

      // Dia+Hora+Centesimas: YYYYMMDDHHMISSCC
      dtc = dt;
      dtc += c;

      assert(dtc.length());
   }
    
    COTDDate(int increment)
    {
        gettimeofday(&tvDate, NULL);
        struct tm *ts = localtime(&(tvDate.tv_sec));
        ts->tm_mday += increment;
        ; /* Normalise ts */
        tvDate.tv_sec = mktime(ts);
    }
    
   std::string format(const char * pFmt)
   {
       char szBuf[128];
       strftime(szBuf, 128, pFmt, localtime(&(tvDate.tv_sec)));
       std::string str(szBuf);
       return str;
   }

   time_t toTime() { return tvDate.tv_sec; }

   /// Constructor con formato especificado

   /**
    * Construir una fecha en base a un formato f
    * @param f formato
    */
   COTDDate(const char * f, std::string &string)
   {
      // Si el formato es Nulo, obtiene una fecha con formato generico
	  if (f != NULL) {
		  string = build(f).c_str();
	  } else {
		  string = build((char *)"%Y-%m-%d %H:%M:%S").c_str();
		  string += ".";
		  string += c.c_str();
	  }
      // Nunca deberia suceder que el size es 0
      assert(string.length());
   }
   

   /**
    * Constructor de fecha
    * @param year
    * @param month
    * @param day
    */
   COTDDate(int year, int month, int day)
   {
      if (validate(year, month, day) == true)
      {
         setOk();
         /// Centesimas: CC
         c = "00";
         /// Date: YYYYMMDD
         {
            std::strstream sAux;
            sAux << year << month << day << '\0';
            d = sAux.str();
         }
         /// Time: HHMISS
         t = "000000";
         /// DateTime: YYYYMMDDHHMISS
         dt = d;
         dt += t;
         /// DateTimeCent: YYYYMMDDHHMISSCC
         dtc = dt;
         dtc += c;
      }
   }
   //@}

   // Metodos publicos
   /**@name public methods */
   //@{
   /// Valida que la fecha se encuentre entre los rangos permitidos de Year,
   // y si es una fecha valida, segun Calendario Gregoriano
   inline bool validate(int year, int month, int day)
   {
      bool r = false;
      do {
         // Valido el year, estos son los limites
         if (year < YearMin || year > YearMax)
            break;

         // Valido el Mes
         if (month < 1 || month > 12)
            break; // ingreso un mes, invalido!

         // Valido el dia
         if (day < 1 || day > 31)
            break; // Ingreso un dia, invalido!

         // Valido los Dias de meses cortos
         if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
            break; // Ese mes no tiene esa cant. de dias

         // Bisiestos con 28 dias en febrero
         if (month == 2 && ((!(year % 4) && day > 29) || ((year % 4) && day > 28)))
            break; // Aqui en la tierra este year, cuando es febrero no tiene esa cant. de dias

         r = true;
         break;
      } while(1);

      return r;
   }

   /**
    * Retorna el dia de la semana.
    * 0: Sunday - 1: Monday - 2: Tuesday - 3: Wednesday - 4: Thursday - 5: Friday - 6: Saturday
    * @param year
    * @param month
    * @param day
    * 
    * @return int
    */
   inline int weekDay(int year, int month, int day)
   {
      int d, m, y, limitemonth;
      // El (primer dia de la semana + 1) en 'YearMin'
      d = firstWeekDay;
      for (y = YearMin; y <= year; y++)
      {
         if (y == year)
         {
            // Tengo que saber cuantos dias transcurrieron hasta el mes
            // anterior, del año que me piden
            limitemonth = (month - 1);
         } else {
            // Los demas años con 12 meses
            limitemonth = 12;
         }
         // Ahora el mes
         for (m = 1; m <= limitemonth; m++)
         {
            // En abril, Junio, Septiembre y Noviembre siempre tengo
            // 30 dias
            if (m == 4 || m == 6 || m == 9 || m == 11)
            {
               d += 30;
            } else if (m == 2) // Si es bisiesto
            {
               // el año es divisible por 4: febrero tiene 29 dias
               if (!(y % 4))
                  d += 29;
               else
                  d += 28; // Febrero con 28 dias
            } else {
               // es un mes con 31 dias
               d += 31;
            }
         }
      }
      // ahora tengo a 'd' con tantos dias antes de llegar al
      // corriente mes/año, entonces le sumo los dias 
      // del corriente mes
      d += day;

      // Esto me da el resto que es igual al dia de la semana
      // el numero de la semana entre 0 y 6
      if (!(d % 7))
         return 6;      
      else
         return ((d % 7) - 1);
   }

   /// return Date
   std::string date()    { return d;   }
   /// return Time
   std::string time()    { return t;   }
   /// return Date-Time
   std::string dateTime()   { return dt;  }
   /// return Date-Time-Cent.
   std::string dateTimeCent()  { return dtc; }
   /// return Cent
   int cent()  { return atoi(c.c_str()); }

   int year() { return atoi(this->d.substr(0, 4).c_str()); }
   int month() { return atoi(this->d.substr(4, 2).c_str()); }
   int day() { return atoi(this->d.substr(6, 2).c_str());}

   //@}

   // Operador publico friend
   friend std::ostream& operator << ( std::ostream& os, COTDDate d ) { os << d.dateTimeCent(); return os; }
};


#endif // __COTD_DATE_H__
