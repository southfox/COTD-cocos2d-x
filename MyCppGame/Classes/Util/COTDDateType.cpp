//
//  COTDDateType.cpp
//
//  Created by Javier Fuchs on 07/25/15.
//  Copyright (c) 2015 Javier Fuchs. All rights reserved.
//

#ifdef _COTDDATETYPE_TEST_
#include <strstream>

#ifndef _ALLOW_STD_TYPES_
#define _ALLOW_STD_TYPES_
#endif
#include "COTDDate.h"

/**
 * Test
 * 
 * @return int
 */
// How to test it:
// $ g++  -Wno-deprecated -I./ -I../../shared/h -DLINUX -D_COTDDATE_TEST_ COTDDate.cpp -o COTDDateTest
// $ ./COTDDateTest

int main()
{
    COTDDateType brkdate;
    // 05-09-2007 17:05:00
    // 2007092116181940
    std::cout << "year: "   ; std::cin >> brkdate.year;
    std::cout << "month: "  ; std::cin >> brkdate.month;
    std::cout << "day: "    ; std::cin >> brkdate.day;
    std::cout << "hour: "   ; std::cin >> brkdate.hour;
    std::cout << "minute: " ; std::cin >> brkdate.minute;
    std::cout << "second: " ; std::cin >> brkdate.second;
    brkdate.setNoNull();

    std::cout << "COTDDate = " << brkdate << " - ";
    time_t ltimet = brkdate.toTime();
    time_t gmtimet = brkdate.toGmtTime(0);
    std::cout << "local = " << ltimet << " - ";
    std::cout << "gmt = " << gmtimet << std::endl;

    COTDDateType local;
    local= ltimet;
    COTDDateType gmt;
    gmt = gmtimet;

    std::cout << "local COTDDate = " << local << " - ";
    std::cout << "gmt COTDDate = " << gmt << std::endl;

    COTDDateType paracompararsysdate;
    COTDDateType fVigencia;
    fVigencia = brkdate;
    paracompararsysdate = fVigencia.toGmtTime(0);
    std::cout << "paracompararsysdate from gmt = " << paracompararsysdate 
	<< " (time_t) L: " << paracompararsysdate.toTime() << " - "
	<< " (time_t) GMT: " << paracompararsysdate.toGmtTime(0) << std::endl;

    COTDDate sysdate;
    std::cout << "sysdate = " << sysdate << " (time_t) " << sysdate.toTime() << std::endl;
    if (fVigencia < sysdate)  { std::cout << "fv < sysdate" << std::endl; }
    if (fVigencia <= sysdate) { std::cout << "fv <= sysdate" << std::endl; }
    if (fVigencia == sysdate) { std::cout << "fv == sysdate" << std::endl; }
    if (fVigencia >= sysdate) { std::cout << "fv >= sysdate" << std::endl; }
    if (fVigencia > sysdate)  { std::cout << "fv > sysdate" << std::endl; }
    std::cout << std::endl;
    if (paracompararsysdate < sysdate)  { std::cout << "fvGmt < sysdate" << std::endl; }
    if (paracompararsysdate <= sysdate) { std::cout << "fvGmt <= sysdate" << std::endl; }
    if (paracompararsysdate == sysdate) { std::cout << "fvGmt == sysdate" << std::endl; }
    if (paracompararsysdate >= sysdate) { std::cout << "fvGmt >= sysdate" << std::endl; }
    if (paracompararsysdate > sysdate)  { std::cout << "fvGmt > sysdate" << std::endl; }

    long timet;
    std::cout << "ingresar time_t: " ; std::cin >> timet;
    COTDDateType d1;
    d1 = timet;
    std::cout << "d1 = " << d1 << std::endl;
    
    return 0;
}



#endif // _COTDDATETYPE_TEST_
