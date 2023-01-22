#ifndef FUNCTION_HANDLER_H
#define FUNCTION_HANDLER_H

#include "ModuleInterfaceHandler.h"

/**
 * @brief Class providing a mapping between integer function codes and
 *        associated functions which support the requirements of
 *        ModuleInterfaceHandler.
 */
class FunctionHandler: public ModuleInterfaceHandler {
  
  public:

    /**
     * @brief Structure mapping an integer functionCode to an associated
     *        handler function.
     */
    typedef struct { unsigned int functionCode; bool (*handler)(unsigned char, unsigned char); } FunctionMap;
    
    /**
     * @brief Construct a new FunctionHandler object.
     * 
     * Zero or more FunctionMap definitions can be added to a
     * FunctionHandler object at instantiation by passing a statically
     * allocated array of FunctionMaps to the constructor.
     * 
     * @example
     * ```
     * FunctionMap myFunctionMap[] = {
     *   { 0, [](unsigned char functionCode, unsigned char value)->bool{ return((value % 2) == 0); } },
     *   { 1, [](unsigned char functionCode, unsigned char value)->bool{ return((value % 2) == 1); } },
     *   { 0, 0 }
     * }
     * FunctionHandler myFunctionHandler(myFunctionMap);
     * ```
     * With a single array argument the FunctionHandler object is sized
     * to accommodate the supplied map array and use of the addHandler()
     * method to dynamically add more function maps is not possible.
     * 
     * Optionally the constructor can be passed a second argument which
     * specifies the maximum size of the FunctionHandler and, so long
     * as the supplied array doesn't fill the map, addHandler() can be
     * used to dynamically add more maps to the FunctionHandler after
     * instantiation.
     * 
     * @example
     * ```
     * FunctionMap myFunctionMap[] = {
     *   { 0, [](unsigned char functionCode, unsigned char value)->bool{ return((value % 2) == 0); } },
     *   { 1, [](unsigned char functionCode, unsigned char value)->bool{ return((value % 2) == 1); } },
     *   { 0, 0 }
     * }
     * FunctionHandler myFunctionHandler(myFunctionMap, 10);
     * myFunctionHandler.addHandler(9, [](unsigned char functionCode, unsigned char value)->bool{ return(value > 99); });
     * 
     * bool isBig = myFunctionHandler.process(9, 101);
     * ```
     * 
     * @param functionMapArray - array of FunctionMap structures or 0.
     * @param size             - the maximum number of FunctionMaps
     *                           that can be saved in FunctionHandler
     *                           or zero (the default) to size
     *                           FunctionHandler so that it can only
     *                           hold @param functionMapArray. 
     */
    FunctionHandler(FunctionMap *functionMapArray, unsigned int size = 0);

    bool addHandler(unsigned char functionCode, bool (*handler)(unsigned char, unsigned char));

    /**
     * @brief Check that a function code is mapped to a function.
     * 
     * @param functionCode - the functiom code to be checked.
     * @return true        - if a FunctionMap exists with the specified function code.
     * @return false       - if there is no FunctionMap with the specified function code.
     */
    bool validateAddress(unsigned char functionCode);

    /**
     * @brief 
     * 
     * @param functionCode - the code of the function to be called.
     * @param value        - a value to be passed to the function.
     * @return true        - the function returned true (whatever that means).
     * @return false       - the function returned false (whatever that means).
     */
    bool processValue(unsigned char functionCode, unsigned char value);

  private:
    unsigned int arraySize;
    FunctionMap *functionMapArray;

};

#endif