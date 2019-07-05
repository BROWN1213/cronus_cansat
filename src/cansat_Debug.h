/**
* @file 
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2019 Asgardia
* @date June 2019
* @brief ...
*/

#ifndef CANSAT_DEBUG_H
#define CANSAT_DEBUG_H


#include <stddef.h>
#include <inttypes.h>
#include <assert.h>

typedef uint32_t millis_time_t;



#define CansatMillis() millis()


#include <Arduino.h>

#include <avr/pgmspace.h>


#define CANSAT_PRINT Serial

// Diagnostic defines

//#define CANSAT_FATAL(msg)     { CANSAT_LOG1(msg); CansatFatal(); }
//#define CANSAT_LOG_RAM()      { CANSAT_LOG2(CANSAT_F("Free RAM: "), CansatFreeRam()); }
//#define CANSAT_LOG_FN()       CANSAT_LOG3(CANSAT_F(__FUNCTION__), '@', __LINE__);
//#define CANSAT_LOG_TROUBLE(t) CANSAT_LOG2(CANSAT_F("Trouble detected: email to cchamchi@naver.com-"), t)

#ifndef CANSAT_PRINT
#undef CANSAT_DEBUG
#endif

#ifdef CANSAT_DEBUG_ALL
#define CANSAT_DEBUG
#endif

#ifdef CANSAT_PRINT

    #define CANSAT_LOG(msg)  {CANSAT_LOG_TIME(); CANSAT_PRINT.println(F(msg));   }

    #define CANSAT_ASSERT(expr) { if(!(expr)) { CANSAT_LOG("Assertion failed."); CANSAT_DBG_BREAK()} }
    
    #define CANSAT_LOG1(p1)            { CANSAT_LOG_TIME(); CANSAT_PRINT.println(p1); }
    #define CANSAT_LOG2(p1,p2)         { CANSAT_LOG_TIME(); CANSAT_PRINT.print(p1); CANSAT_PRINT.print(F(",")); CANSAT_PRINT.println(p2); }
    #define CANSAT_LOG3(p1,p2,p3)      { CANSAT_LOG_TIME(); CANSAT_PRINT.print(p1); CANSAT_PRINT.print(F(",")); CANSAT_PRINT.print(p2); CANSAT_PRINT.print(F(",")); CANSAT_PRINT.println(p3); }
    #define CANSAT_LOG4(p1,p2,p3,p4)   { CANSAT_LOG_TIME(); CANSAT_PRINT.print(p1); CANSAT_PRINT.print(F(",")); CANSAT_PRINT.print(p2); CANSAT_PRINT.print(F(",")); CANSAT_PRINT.print(p3); CANSAT_PRINT.print(F(","));CANSAT_PRINT.println(p4); }


        static
        void CANSAT_LOG_TIME() {
            CANSAT_PRINT.print(F("["));
            CANSAT_PRINT.print(millis());
            CANSAT_PRINT.print(F("] "));
        }

#ifdef CANSAT_DEBUG
        #define CANSAT_DBG_DUMP(msg, addr, len) if (len) { CANSAT_PRINT.print(F(msg)); CANSAT_PRINT.write((uint8_t*)addr, len); CANSAT_PRINT.println(); }
        #define CANSAT_DBG_BREAK()    { for(;;); }
#endif


#endif //#ifdef CANSAT_PRINT

#ifndef CANSAT_LOG
    #define CANSAT_LOG(...)
    #define CANSAT_LOG1(...)
    #define CANSAT_LOG2(...)
    #define CANSAT_LOG3(...)
    #define CANSAT_LOG4(...)


#endif

#ifndef CANSAT_DBG_BREAK
    #define CANSAT_DBG_BREAK()
    #define CANSAT_ASSERT(expr)
    #define CANSAT_DBG_DUMP(msg, addr, len)
#endif



#endif