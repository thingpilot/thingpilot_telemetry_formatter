/**
  * @file    tformatter.cpp
  * @version 0.1.0
  * @author  Adam Mitchell
  * @brief   C++ file of the thingpilot telemetry formatter
  */

/** Includes
 */
#include "tformatter.h"

/** Class constructor
 */
TFormatter::TFormatter()
{

}

/** Class destructor
 */
TFormatter::~TFormatter()
{

}


int TFormatter::serialise(char *input,  uint16_t input_length_bytes,
                          char *output, uint16_t output_length_bytes)
{
	memcpy(output, input, output_length_bytes);

	return TFormatter::TFORMATTER_OK;
}


int TFormatter::deserialise(char *serialised,   uint16_t serialised_length_bytes,
                            char *deserialised, uint16_t deserialised_length_bytes)
{
	memcpy(deserialised, serialised, deserialised_length_bytes);

	return TFormatter::TFORMATTER_OK;
}