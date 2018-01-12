#ifndef status_h
#define status_h

typedef struct owd {
        uint8_t id[8];
} owd_t ;

typedef struct status {
        uint16_t status_size;
        uint8_t ds_info;
        uint8_t ds_busError;
        uint8_t dht_info;
        uint8_t dht_busError;
        uint8_t mpx_info;
        uint8_t mpx_busError;
        uint8_t in_info;
        uint8_t out_info;
} status_t;

#endif
