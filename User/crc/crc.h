#ifndef  CRC16_H
#define  CRC16_H
#include <stdint.h>

#ifdef __cplusplus  
extern "C" {
#endif 

uint16_t crc_ccitt(const void *data, uint16_t data_size);

#ifdef __cplusplus  
}
#endif

#endif // ! CRC16_H
