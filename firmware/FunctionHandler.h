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
     * @brief Construct a new Function Handler object.
     * 
     * @example The following example maps function code 0 onto the even
     * function and function code 1 onto odd.
     * ```
     * FunctionMap myFunctionMap[] = {
     *   { 0, [](unsigned char functionCode, unsigned char value)->bool{ return((value % 2) == 0); } },
     *   { 1, [](unsigned char functionCode, unsigned char value)->bool{ return((value % 2) == 1); } },
     *   { 0, 0 }
     * }
     * FunctionHandler myFunctionHandler(myFunctionMap);
     * 
     * bool isEven = myFunctionHandler.process(0, 10);
     * bool isOdd = myFunctionHandler.process(1, 10);
     * ```
     * 
     * @param functionMapArray - array of FunctionMap structures. 
     */
    FunctionHandler(FunctionMap *functionMapArray);

    /**
     * @brief Check that a function code is mapped to a function.
     * 
     * @param functionCode - the functiom code to be checked.
     * @return true        - if a FunctionMap exists with the specified function code.
     * @return false       - if there is no FunctionMap with the specified function code.
     */
    bool validate(unsigned char functionCode);

    /**
     * @brief 
     * 
     * @param functionCode - the code of the function to be called.
     * @param value        - a value to be passed to the function.
     * @return true        - the function returned true (whatever that means).
     * @return false       - the function returned false (whatever that means).
     */
    bool process(unsigned char functionCode, unsigned char value);

  private:
    FunctionMap *functionMapArray;

};

#endif