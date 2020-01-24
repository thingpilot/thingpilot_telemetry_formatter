/**
  * @file    tformatter.cpp
  * @version 0.2.0
  * @author  Rafaella Neofytou, Adam Mitchell
  * @brief   C++ file of the thingpilot telemetry formatter. Formats timeseries and instantaneous data for thingpilot Cloud API Consumption
  *          This will format the data to different formats CBOR, etc.
  */

/** Includes
 */
#include "tformatter.h"

/** Class constructor
 */
TFormatter::TFormatter()
{
     general_cbor_array = NULL;
}

/** Class destructor
 */
TFormatter::~TFormatter()
{
    if (general_cbor_array != NULL)
    {
        delete [] general_cbor_array;
    }
}


void TFormatter::setup()
{
    #if BOARD == EARHART_V1_0_0 
        general_cbor_array= new uint8_t[255];
    #endif
    #if BOARD == WRIGHT_V1_0_0
        general_cbor_array= new uint8_t[1600];
    #endif
    r_entries=0;
    entries=1;
}


void TFormatter::serialise_main_cbor_object()
{  
    general_cbor_array[0]=163;
    write_string("unique_id"); 
    write_string(app_id);
    write_string("project_id"); 
    write_string(dev_id);
    write_string("metricGroups");
}

void TFormatter::get_final_serialised(uint8_t* buffer,size_t& buffer_len)
{
    for (int i=0; i<entries; i++)
    {
        buffer[i]=general_cbor_array[i];
    }
    buffer_len=entries;
    r_entries=0;
    entries=1;
}

void TFormatter::get_serialised(uint8_t* buffer,size_t& buffer_len)
{
    _write_timestamp();
    general_cbor_array[0]=161+r_entries;

    for (int i=0; i<entries; i++)
    {
        buffer[i]=general_cbor_array[i];
    }
    buffer_len=entries;
    r_entries=0;
    entries=1;
}

void TFormatter::write(uint8_t num, int cbor_codes)
{   
    general_cbor_array[entries]=num+cbor_codes;
    entries++;
}

void TFormatter::write_string(const string& object_str)
{
    write(object_str.length(), TFormatter::TEXT);
    for (size_t i = 0; i < object_str.length(); ++i)
    {
        write(object_str[i], TFormatter::RAW);
    }
}

void TFormatter::_write_timestamp()
{
    write_string("time"); 
    write(TFormatter::RAW, TFormatter::TIME_TAG);
    write(TFormatter::RAW, TFormatter::INT4); 
    time_t time_now=time(NULL);
    uint8_t time_bytes[4]; 
    *(time_t *)(time_bytes)=time_now;
    for (int i=0; i<4; i++)
    {
        write(time_bytes[3-i], TFormatter::RAW);
    }
}

/* */
void TFormatter::get_entries(uint16_t& c_entries)
{
    c_entries=entries;
}


template<typename T>
void TFormatter::_get_type(uint8_t& _data_type, T data)
{ 
    _data_type=TFormatter::RAW;
};


template<> void TFormatter::_get_type<int8_t>(uint8_t& _data_type, int8_t _data)
{ 
    if(_data>0)
    {
        _data_type=TFormatter::INT1;
    }
    else
    {
        _data_type=TFormatter::INT1N;
    }
};
template<> void TFormatter::_get_type<int16_t>(uint8_t& _data_type,int16_t _data)
{ 
    if(_data>0)
    {
        _data_type=TFormatter::INT2;
    }
    else
    {
        _data_type=TFormatter::INT2N;
    } 
};
template<> void TFormatter::_get_type<int32_t>(uint8_t& _data_type,int32_t _data)
{ 
    if(_data>0)
    {
        _data_type=TFormatter::INT4;
    }
    else
    {
        _data_type=TFormatter::INT4N;
    } 
};
template<> void TFormatter::_get_type<int64_t>(uint8_t& _data_type,int64_t _data)
{ 
    if(_data>0)
    {
        _data_type=TFormatter::INT8; 
    }
    else
    {
        _data_type=TFormatter::INT8N;
    } 
};
template<> void TFormatter::_get_type<uint8_t>(uint8_t& _data_type, uint8_t _data){ _data_type=TFormatter::INT1; };
template<> void TFormatter::_get_type<uint16_t>(uint8_t& _data_type, uint16_t _data){ _data_type=TFormatter::INT2; };
template<> void TFormatter::_get_type<uint32_t>(uint8_t& _data_type, uint32_t _data){ _data_type=TFormatter::INT4; };
template<> void TFormatter::_get_type<uint64_t>(uint8_t& _data_type, uint64_t _data){ _data_type=TFormatter::INT8; };
template<> void TFormatter::_get_type<float>(uint8_t& _data_type,float _data){ _data_type=TFormatter::FLOAT; };

template <typename DataType> 
void TFormatter::write_num_type(DataType input)
{
    r_entries++;
    uint8_t _value=0;
    _get_type<DataType>(_value,input);
    write(_value, TFormatter::RAW); 
    if (input<0)
    {
        input=-1-input;
    }
    uint8_t bytes[sizeof(DataType)];
    *(DataType *)(bytes)=input;
    for (int i=sizeof(DataType); i>0; i--)
    {
        write(bytes[i-1], TFormatter::RAW);
    }

}
template void TFormatter::write_num_type<float>(float input);
template void TFormatter::write_num_type<int>(int input);
template void TFormatter::write_num_type<int8_t>(int8_t input);
template void TFormatter::write_num_type<int16_t>(int16_t input);
template void TFormatter::write_num_type<int64_t>(int64_t input);
template void TFormatter::write_num_type<uint8_t>(uint8_t input);
template void TFormatter::write_num_type<uint16_t>(uint16_t input);
template void TFormatter::write_num_type<uint32_t>(uint32_t input);
template void TFormatter::write_num_type<uint64_t>(uint64_t input);
