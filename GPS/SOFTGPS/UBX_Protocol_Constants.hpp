#ifndef UBX_PROTOCOL_H
#define UBX_PROTOCOL_H

/*GLOBAL VARIABLES*/
namespace UBX_protocol
{
    /*NMEA*/ // disable SBAS QZSS GLONASS BeiDou Galileo
    inline constexpr char NMEA_CFG[] = "\xB5\x62\x06\x17\x0C\x00\x00\x23\x00\x02\x76\x00\x00\x00\x00\x00\x00\x00\xC4\xA4";
    inline constexpr size_t NMEA_CFG_Length = 21; // sizeof NMEA / sizeof NMEA[0]; // length of NMEA CFG Message

    /*RATE*/ // Measurement frequency: 10 hz, navigation frequency 10 hz
    // inline constexpr char RATE[] = "\xB5\x62\x06\x08\x06\x00\x64\x00\x01\x00\x01\x00\x7A\x12";
    inline constexpr char RATE[] = "\xB5\x62\x06\x08\x06\x00\xC8\x00\x01\x00\x01\x00\xDE\x6A";
    inline constexpr size_t RATE_Length = 15; // sizeof RATE / sizeof RATE[0]; // length of RATE CFG Message

    /*BAUDRATE 4800*/
    inline constexpr char BAUD[] = "\xB5\x62\x06\x00\x14\x00\x01\x00\x00\x00\xD0\x08\x00\x00\xC0\x12\x00\x00\x03\x00\x03\x00\x00\x00\x00\x00\xCB\xC4";
    inline constexpr size_t BAUD_Length = 29; // sizeof BAUD / sizeof BAUD[0]; // length of BAUD CFG Message
    /*NMEA MESSAGES*/
    inline constexpr char GLL[] = "\xB5\x62\x06\x01\x08\x00\xF0\x01\x00\x00\x00\x00\x00\x00\x00\x2A"; // disable GPGLL
    inline constexpr char GSA[] = "\xB5\x62\x06\x01\x08\x00\xF0\x02\x00\x00\x00\x00\x00\x00\x01\x31"; // disable GSA
    inline constexpr char GSV[] = "\xB5\x62\x06\x01\x08\x00\xF0\x03\x00\x00\x00\x00\x00\x00\x02\x38"; // disable GPGSV
    inline constexpr char RMC[] = "\xB5\x62\x06\x01\x08\x00\xF0\x04\x00\x00\x00\x00\x00\x00\x03\x3F"; // disable RMC
    inline constexpr char VTG[] = "\xB5\x62\x06\x01\x08\x00\xF0\x05\x00\x00\x00\x00\x00\x00\x04\x46"; // disable VTG
    inline constexpr size_t GP_Length = 17;

    /*SAVE*/
    inline constexpr char SAFE[] = "\xB5\x62\x06\x09\x0D\x00\x00\x00\x00\x00\xFF\xFF\x00\x00\x00\x00\x00\x00\x17\x31\xBF";
    inline constexpr size_t SAFE_Length = 22;
}
#endif