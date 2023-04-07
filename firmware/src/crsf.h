/* 
 * File:   crsf.h
 * Author: bemcg
 *
 * Created on March 9, 2023, 11:41 AM
 */

#ifndef CRSF_H
#define	CRSF_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MAX_CRSF_PACKET     64
#define CHANNEL_PACKET_LEN  26  //3 bytes header, 22 bytes channels, 1 byte CRC

    enum {
        CRSF_ADDRESS_BROADCAST = 0x00,
        CRSF_ADDRESS_USB = 0x10,
        CRSF_ADDRESS_TBS_CORE_PNP_PRO = 0x80,
        CRSF_ADDRESS_RESERVED1 = 0x8A,
        CRSF_ADDRESS_CURRENT_SENSOR = 0xC0,
        CRSF_ADDRESS_GPS = 0xC2,
        CRSF_ADDRESS_TBS_BLACKBOX = 0xC4,
        CRSF_ADDRESS_FLIGHT_CONTROLLER = 0xC8,
        CRSF_ADDRESS_RESERVED2 = 0xCA,
        CRSF_ADDRESS_RACE_TAG = 0xCC,
        CRSF_ADDRESS_RADIO_TRANSMITTER = 0xEA,
        CRSF_ADDRESS_CRSF_RECEIVER = 0xEC,
        CRSF_ADDRESS_CRSF_TRANSMITTER = 0xEE,
    };

    enum {
        CRSF_FRAMETYPE_GPS = 0x02,
        CRSF_FRAMETYPE_BATTERY_SENSOR = 0x08,
        CRSF_FRAMETYPE_BARO_ALTITUDE = 0x09,
        CRSF_FRAMETYPE_LINK_STATISTICS = 0x14,
        CRSF_FRAMETYPE_OPENTX_SYNC = 0x10,
        CRSF_FRAMETYPE_RADIO_ID = 0x3A,
        CRSF_FRAMETYPE_RC_CHANNELS_PACKED = 0x16,
        CRSF_FRAMETYPE_ATTITUDE = 0x1E,
        CRSF_FRAMETYPE_FLIGHT_MODE = 0x21,
        // Extended Header Frames, range: 0x28 to 0x96
        CRSF_FRAMETYPE_DEVICE_PING = 0x28,
        CRSF_FRAMETYPE_DEVICE_INFO = 0x29,
        CRSF_FRAMETYPE_PARAMETER_SETTINGS_ENTRY = 0x2B,
        CRSF_FRAMETYPE_PARAMETER_READ = 0x2C,
        CRSF_FRAMETYPE_PARAMETER_WRITE = 0x2D,
        CRSF_FRAMETYPE_COMMAND = 0x32,
        // MSP commands
        CRSF_FRAMETYPE_MSP_REQ = 0x7A, // response request using msp sequence as command
        CRSF_FRAMETYPE_MSP_RESP = 0x7B, // reply with 58 byte chunked binary
        CRSF_FRAMETYPE_MSP_WRITE = 0x7C, // write with 8 byte chunked binary (OpenTX outbound telemetry buffer limit)
    };

    typedef struct {
        uint8_t address;
        uint8_t length;
        uint8_t type;
        uint8_t data[0];
    } CRSF_Frame;

    typedef struct __attribute((packed)) {
        unsigned ch0 : 11;
        unsigned ch1 : 11;
        unsigned ch2 : 11;
        unsigned ch3 : 11;
        unsigned ch4 : 11;
        unsigned ch5 : 11;
        unsigned ch6 : 11;
        unsigned ch7 : 11;
        unsigned ch8 : 11;
        unsigned ch9 : 11;
        unsigned ch10 : 11;
        unsigned ch11 : 11;
        unsigned ch12 : 11;
        unsigned ch13 : 11;
        unsigned ch14 : 11;
        unsigned ch15 : 11;
    }
    CRSF_ChannelData;

    typedef struct {
        uint8_t uplink_RSSI_1;
        uint8_t uplink_RSSI_2;
        uint8_t uplink_Link_quality;
        int8_t uplink_SNR;
        uint8_t active_antenna;
        uint8_t rf_Mode;
        uint8_t uplink_TX_Power;
        uint8_t downlink_RSSI;
        uint8_t downlink_Link_quality;
        int8_t downlink_SNR;
    } CRSF_LinkStatistics;

    typedef struct __attribute((packed)) {
        unsigned voltage : 16; // mv * 100 BigEndian
        unsigned current : 16; // ma * 100
        unsigned capacity : 24; // mah
        unsigned remaining : 8; // %
    }
    CRSF_sensor_battery;

    typedef struct __attribute((packed)) {
        uint16_t altitude; // Altitude in decimeters + 10000dm, or Altitude in meters if high bit is set, BigEndian
        int16_t verticalspd; // Vertical speed in cm/s, BigEndian
    }
    CRSF_sensor_baro_vario;

    void initCRSF(void);
    uint8_t calcCRC(uint8_t *data, uint8_t len);
    void sendBatteryTelem(float voltage);
    void sendAltitudeTelem(float altitude, float vSpeed);

#ifdef	__cplusplus
}
#endif

#endif	/* CRSF_H */

