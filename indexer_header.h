#ifndef _INDEXER_HEADER_H
#define _INDEXER_HEADER_H
#include "headers.h"

#define KEY_SIZE 32
#define VALUE_SIZE_26 (SUBSEQ_SIZE - KEY_SIZE - 6)
#define VALUE_SIZE_24 (SUBSEQ_SIZE - KEY_SIZE - 8)

typedef struct keyBitsetStruct{
    bitset<KEY_SIZE> keyBits;
    bool operator<(const keyBitsetStruct& keyBitsTemp) const{
        return keyBits.to_string() < keyBitsTemp.keyBits.to_string();
    }
}keyBitsetStruct;

typedef struct valueBitsetStruct_24{
    bitset<VALUE_SIZE_24> valueBits_24;
    bool operator<(const valueBitsetStruct_24& valueBitsTemp_24) const{
        return valueBits_24.to_string() < valueBitsTemp_24.valueBits_24.to_string();
    }
}valueBitsetStruct_24;

typedef struct valueBitsetStruct_26{
    bitset<VALUE_SIZE_26> valueBits_26;
    bool operator<(const valueBitsetStruct_26& valueBitsTemp_26) const{
        return valueBits_26.to_string() < valueBitsTemp_26.valueBits_26.to_string();
    }
}valueBitsetStruct_26;

extern map<keyBitsetStruct , map<valueBitsetStruct_24 , list<uint32_t> > > indexerMap_24 ;
extern map<keyBitsetStruct , map<valueBitsetStruct_26 , list<uint32_t> > > indexerMap_26 ;

void parseReadPacket(unsigned char *);
void parseRefPacket(unsigned char *);

#endif
