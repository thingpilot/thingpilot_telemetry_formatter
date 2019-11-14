/**
  * @file    tformatter.h
  * @version 0.1.0
  * @author  Adam Mitchell
  * @brief   Header file of the thingpilot telemetry formatter 
  */

/** Define to prevent recursive inclusion
 */
#pragma once

/** Includes 
 */
#include <mbed.h>

/** Base class for the Thingpilot telemetry formatter
 */
class TFormatter
{

	public:

		/** Function return codes
		 */
		enum 
		{
			TFORMATTER_OK  = 0,
			INVALID_FORMAT = 1
		}

		/** Enumerated list of formats to which TFormatter can
		 *  serialise data
		 * 
		 * @RAW Do not encode the payload
		 * @CBOR Encode payload as CBOR
		 */
		enum class Formats
		{
			RAW  = 0,
			CBOR = 1
		};

		/** Class constructor
		 */
		TFormatter();

		/** Class destructor
		 */
		~TFormatter();

		int TFormatter::serialise(char *input,  uint16_t input_length_bytes,
                         	      char *output, uint16_t output_length_bytes);

		int TFormatter::deserialise(char *serialised,   uint16_t serialised_length_bytes,
                            		char *deserialised, uint16_t deserialised_length_bytes);
};
