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
    _is_initialised=false;  
}

/** Class destructor
 */
TFormatter::~TFormatter()
{
    delete [] general_cbor_array;
}


void TFormatter::setup()
{
    if(!_is_initialised)
    {
        general_cbor_array= new uint8_t[TP_TX_BUFFER + 100]; 
        r_entries=0;
        entries=1;
        _is_initialised=true;
    }
}

void TFormatter::serialise_main_cbor_object(uint8_t num)
{  
    general_cbor_array[0] = 163;
    write_string("uniqueId"); 
    write_string(UID);
    write_string("deviceType_id"); 
    write_string(DEVID);
    write_string("metrics");
    write(num, MAP);
    
}

uint8_t * TFormatter::return_serialised(size_t& buffer_len)
{
    buffer_len=entries;
    r_entries=0;
    entries=1;

    return general_cbor_array;
}

void TFormatter::get_serialised(uint8_t* buffer,size_t& buffer_len)
{
   // _write_timestamp();
    general_cbor_array[0] = TFormatter::MAP + 3; //161

    for (int i=0; i < entries; i++)
    {
        buffer[i] = general_cbor_array[i];
    }
    buffer_len=entries;
    r_entries=0;
    entries=1;
}

void TFormatter::write(uint8_t num, int cbor_codes)
{   
    general_cbor_array[entries]= num + cbor_codes;
    entries++;
}

void TFormatter::write_string(const string& object_str)
{
   
   // cout << object_str.length() << endl;
    uint8_t len = object_str.length();
    if (len > 23)
    {
        general_cbor_array[entries]= 120;//17 + TFormatter::TEXT;
        general_cbor_array[entries + 1] = object_str.length(); //- 17 + TFormatter::INT1;
        entries +=2 ;
    }
    else 
    {
        write(len, TFormatter::TEXT);
    }
    for (size_t i = 0; i < len; ++i)
    {
        write(object_str[i], TFormatter::RAW);
    }
}

void TFormatter::_write_timestamp()
{
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

void TFormatter::increase_entries()
{
    entries++;
}

void TFormatter::decrease_entries()
{
    entries--;
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

template<> void TFormatter::_get_type<bool>(uint8_t& _data_type,bool _data)
{ 
    if(_data==true)
    {
        _data_type=TFormatter::B_TRUE; 
    }
    else
    {
        _data_type=TFormatter::B_FALSE;
    } 
};

template<> void TFormatter::_get_type<uint8_t>(uint8_t& _data_type, uint8_t _data){ _data_type=TFormatter::INT1; };
template<> void TFormatter::_get_type<uint16_t>(uint8_t& _data_type, uint16_t _data){ _data_type=TFormatter::INT2; };
template<> void TFormatter::_get_type<uint32_t>(uint8_t& _data_type, uint32_t _data){ _data_type=TFormatter::INT4; };
template<> void TFormatter::_get_type<uint64_t>(uint8_t& _data_type, uint64_t _data){ _data_type=TFormatter::INT8; };
template<> void TFormatter::_get_type<float>(uint8_t& _data_type,float _data){ _data_type=TFormatter::FLOAT; };
template<> void TFormatter::_get_type<double>(uint8_t& _data_type,double _data){ _data_type=TFormatter::DOUBLE; };

template <typename DataType> 
void TFormatter::write_num_type(DataType input)
{
    r_entries++;
    uint8_t _value=0;
    _get_type<DataType>(_value,input);
    write(_value, TFormatter::RAW); 
    if (input<0 && _value!=TFormatter::FLOAT && _value!=TFormatter::DOUBLE)
    {
        input=-1-input;
    }
    if (_value != TFormatter::B_TRUE || _value != TFormatter::B_FALSE)
    {
        uint8_t bytes[sizeof(DataType)];
        *(DataType *)(bytes)=input;
        for (int i=sizeof(DataType); i>0; i--)
        {
            write(bytes[i-1], TFormatter::RAW);
        }
    }
}
template void TFormatter::write_num_type<int8_t>(int8_t input);
template void TFormatter::write_num_type<int16_t>(int16_t input);
template void TFormatter::write_num_type<int32_t>(int32_t input);
template void TFormatter::write_num_type<int64_t>(int64_t input);
template void TFormatter::write_num_type<uint8_t>(uint8_t input);
template void TFormatter::write_num_type<uint16_t>(uint16_t input);
template void TFormatter::write_num_type<uint32_t>(uint32_t input);
template void TFormatter::write_num_type<uint64_t>(uint64_t input);
template void TFormatter::write_num_type<float>(float input);
template void TFormatter::write_num_type<double>(double input);

TFormatter tformatter;

template <typename DataType> 
void add_record(DataType data, string str)
{   
    if (!str.empty()) //size() == 0
    {
        tformatter.write_string(str);
    }
    tformatter.write_num_type<DataType>(data);
}
template void add_record<int8_t>(int8_t data, string str);
template void add_record<int16_t>(int16_t data, string str);
template void add_record<int32_t>(int32_t data, string str);
template void add_record<int64_t>(int64_t data, string str);
template void add_record<uint8_t>(uint8_t data, string str);
template void add_record<uint16_t>(uint16_t data, string str);
template void add_record<uint32_t>(uint32_t data, string str);
template void add_record<uint64_t>(uint64_t data, string str);
template void add_record<float>(float data, string str);
template void add_record<double>(double data, string str);


void TFormatter::add_metric0(float lat, float lon, const string time_str)
{
    write_string("0");
    write(1, TFormatter::ARRAY);
    write(2, TFormatter::MAP);
    
    write_string("0");
    write(1, TFormatter::ARRAY);
    write_string("IS");

    write_string("t");
    write(1, TFormatter::ARRAY);

    write(20, TFormatter::TEXT);
    for (size_t i = 0; i < 20; ++i)
    {
        write(time_str[i], TFormatter::RAW);
    }

    write_string("1");
    write(1, TFormatter::ARRAY);
    write(3, TFormatter::MAP);

    write_string("0");
    write(1, TFormatter::ARRAY);
    write_num_type<float>(lat);

    write_string("1");
    write(1, TFormatter::ARRAY);
    write_num_type<float>(lon);

    write_string("t");
    write(1, TFormatter::ARRAY);
    write(20, TFormatter::TEXT);

    for (size_t i = 0; i < 20; ++i)
    {
        write(time_str[i], TFormatter::RAW);
    }

}
