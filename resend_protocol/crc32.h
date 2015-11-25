#ifndef CRC32_H
#define CRC32_H

unsigned short mycrc16(unsigned char const *buffer, int len);
unsigned int mycrc32(unsigned int crc, unsigned char* p , int len);

#endif
