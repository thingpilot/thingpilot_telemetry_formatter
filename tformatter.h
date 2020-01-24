/**
  * @file    tformatter.h
  * @version 0.2.0
  * @author  Rafaella Neofytou, Adam Mitchell
  * @brief   Header file of the thingpilot telemetry formatter for CBOR (Concise Binary Object Representation)
  *          Followed an \internet Standards Track document for CBOR codes https://tools.ietf.org/html/rfc7049 
  */

/** Define to prevent recursive inclusion
 */
#pragma once

/** Includes 
 */
#include <mbed.h>
#include <string>
#include "config_device.h"

/** Base class for the Thingpilot telemetry formatter
 */
class TFormatter
{

	public:

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
        /** Cbor Major Type Codes. Enumerated list of codes to which TFormatter can
		 *  add to the data. 
         *  @RAW        No code added
         *  @INT1       Unsigned integer (one-byte uint8_t follows)
         *  ..
         *  @INT1N      Signed integer (one-byte uint8_t follows) / Negative
         *  @TEXT       0x60..0x77,UTF-8 string (0x00..0x17 bytes follow)
         *  @ARRAY      array (0x00..0x17 data items follow)
         *  @MAP        map (0x00..0x17 pairs of data items follow)
         *  @GROUP_TAG  0xc6..0xd4(tagged item)
         *  @TIME_TAG   Epoch-based date/time
         *  @FLOAT      Half-Precision Float   
		 */
        enum CBOR_CODES : uint8_t
		{
			RAW             = 0,
            INT1            = 24, //1 byte
            INT2            = 25,
            INT4            = 26,
            INT8            = 27,
            INT1N           = 56, //1 byte
            INT2N           = 57,
            INT4N           = 58,
            INT8N           = 59,
            TEXT            = 96,
			ARRAY           = 128,
            MAP             = 160,
            TIME_TAG        = 193,
            GROUP_TAG       = 197, 
            FLOAT           = 249
		};

		/** Class constructor
		 */
		TFormatter();

		/** Class destructor
		 */
		~TFormatter();
        
        /** Create an array 
         */
        void setup();
        /** Return a buffer of general_cbor_array added main cbor objects 
         */
        void get_final_serialised(uint8_t* buffer,size_t& buffer_len);
        
        /**Add the Cbor main object 
         */
        void serialise_main_cbor_object();
        
        /** Return a buffer of general_cbor_array payload only
         */
        void get_serialised(uint8_t* buffer,size_t& buffer_len);
        
        /** Write a num with a CBOR  major type
         */
        void write(uint8_t num, int cbor_codes);
        
        /** Write a string with CBOR TEXT major type
         */
        void write_string(const string& object_str);
        
        /** Get total entries
         */
        void get_entries(uint16_t& c_entries);
        
        /** Determine wich major type is the user using & save
         */
        template <typename DataType> 
        void write_num_type(DataType _input);

        private:
        uint8_t* general_cbor_array;
        uint16_t r_entries, entries;
        
        /** Determine wich major type is the user using & save
         */
        template <typename T>
        void _get_type(uint8_t& _data_type, T _data);
        
        /**Adds the timestamp.
         * Tag value 0 is for date/time strings that follow the standard format
         * described in [RFC3339]. */
        void _write_timestamp();
        
		// int TFormatter::deserialise(char *serialised,   uint16_t serialised_length_bytes,
        //                     		char *deserialised, uint16_t deserialised_length_bytes);
};
