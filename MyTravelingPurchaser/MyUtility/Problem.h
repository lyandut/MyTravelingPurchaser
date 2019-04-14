////////////////////////////////
/// usage : 1.	data that identifies a guillotine cut problem and its solution.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SMART_SZX_INVENTORY_ROUTING_PROBLEM_H
#define SMART_SZX_INVENTORY_ROUTING_PROBLEM_H


#include "Config.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Common.h"
#include "../TPPLIBInstance/TPPLIBInstance.h"
#include "../Main/TPPCommon.h"


namespace szx {

class Problem {
    #pragma region Type
public:
	using Input = TPPLIBInstance *;
	using Output = Solution;
    #pragma endregion Type

    #pragma region Constant
public:
    static constexpr Price MaxCost = (1 << 30);
    static constexpr double CheckerObjScale = 1000;
    #pragma endregion Constant

    #pragma region Constructor
public:
    #pragma endregion Constructor

    #pragma region Method
public:
    #pragma endregion Method

    #pragma region Field
public:
    #pragma endregion Field
}; // Problem

}


#endif // SMART_SZX_INVENTORY_ROUTING_PROBLEM_H
