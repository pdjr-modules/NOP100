#ifndef FUNCTION_HANDLER_H
#define FUNCTION_HANDLER_H

#include "ModuleInterfaceHandler.h"

/**
 * @brief Object providing a mapping between integer function codes and
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
     * @example The following example maps function code 0 onto the identity
     * function and function code 1 onto double.
     * ```
     * FunctionMap myFunctionMap[] = {
     *   { 0, [](unsigned char functionCode, unsigned char value){ return(value); } },
     *   { 1, [](unsigned char functionCode, unsigned char value){ return(value + value); } },
     *   { 0, 0 }
     * }
     * FunctionHandler myFunctionHandler(myFunctionMap);
     * 
     * int ident = myFunctionHandler.process(0, 10);
     * int double = myFunctionHandler.process(1, 5);
     * ```
     * 
     * @param functionMapArray - array of FunctionMap structures. 
     */
    FunctionHandler(FunctionMap *functionMapArray);

    /**
     * @brief Check that a function code is mapped to a function.
     * 
     * @param functionCode - the code to be checked.
     * @return true        - if FunctionMap exists with the specified function code.
     * @return false       - if there is no FunctionMap with the specified function code.
     */
    bool validate(unsigned char functionCode);

    /**
     * @brief 
     * 
     * @param functionCode 
     * @param value 
     * @return true 
     * @return false 
     */
    bool process(unsigned char functionCode, unsigned char value);

  private:
    FunctionMap *functionMapArray;

};

#endif