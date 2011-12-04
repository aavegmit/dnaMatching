#include "headers.h"
#include "indexer_header.h"

bitset<KEY_SIZE> key;
bitset<VALUE_SIZE_24> value_24;
bitset<VALUE_SIZE_26> value_26;

map<keyBitsetStruct , map<valueBitsetStruct_24 , list<uint32_t> > > indexerMap_24 ;
map<keyBitsetStruct , map<valueBitsetStruct_26 , list<uint32_t> > > indexerMap_26 ;

void printIndexerMap_26(){
    cout<<"Printing indexerMap_26"<<endl;
    for(map<keyBitsetStruct , map<valueBitsetStruct_26 , list<uint32_t> > >::iterator it1 = indexerMap_26.begin(); it1!=indexerMap_26.end(); it1++){
        cout<< "Key inserted in Map: " << (*it1).first.keyBits.to_string() << endl;
        for(map<valueBitsetStruct_26, list<uint32_t> >::iterator it2 = (*it1).second.begin(); it2!=(*it1).second.end();it2++){
            cout<< "subpart of key : " << (*it2).first.valueBits_26.to_string() << endl;
            cout<< "offsets are: ";
            for(list<uint32_t>::iterator it3 = (*it2).second.begin();it3 != (*it2).second.end();it3++){
                cout<<*it3<<" ";
            }
        }
        cout<< endl;
    }
}


void printIndexerMap_24(){
    cout<<"Printing indexerMap_24"<<endl;
    for(map<keyBitsetStruct , map<valueBitsetStruct_24 , list<uint32_t> > >::iterator it1 = indexerMap_24.begin(); it1!=indexerMap_24.end(); it1++){
        cout<< "Key inserted in Map: " << (*it1).first.keyBits.to_string() << endl;
        for(map<valueBitsetStruct_24, list<uint32_t> >::iterator it2 = (*it1).second.begin(); it2!=(*it1).second.end();it2++){
            cout<< "subpart of key : " << (*it2).first.valueBits_24.to_string() << endl;
            cout<< "offsets are: ";
            for(list<uint32_t>::iterator it3 = (*it2).second.begin();it3 != (*it2).second.end();it3++){
                cout<<*it3<<" ";
            }
        }
        cout<< endl;
    }
}

void storeInIndexerMap_26(bitset<KEY_SIZE> key, bitset<VALUE_SIZE_26> value, uint32_t offset){
    keyBitsetStruct objKey;
    objKey.keyBits = key;
    
    valueBitsetStruct_26 objValue;
    objValue.valueBits_26 = value;

    (indexerMap_26[objKey])[objValue].push_back(offset);
}

void storeInIndexerMap_24(bitset<KEY_SIZE> key, bitset<VALUE_SIZE_24> value, uint32_t offset){

    keyBitsetStruct objKey;
    objKey.keyBits = key;
    
    valueBitsetStruct_24 objValue;
    objValue.valueBits_24 = value;

    (indexerMap_24[objKey])[objValue].push_back(offset);
}

//parses the read packet
void parseReadPacket(unsigned char *buffer){
}

// Parses the reference packet
void parseRefPacket(unsigned char *buffer){

    int subseq_length = 56;
    bool subseq_length_flag = false;

    int header_length = sizeof(struct frame);
    struct content_ref *data = (struct content_ref *)(buffer+header_length); 
    
    //cout << "Data Recvd is: " << data->subseq.to_string() << endl;


    if(data->subseq.test(SUBSEQ_SIZE-1)){
        subseq_length = 58;
        subseq_length_flag = true;
    }

    for(int i = KEY_SIZE-1;i>=0;i--){
        key[i] = data->subseq[(subseq_length-1)-((KEY_SIZE-1)-i)];
    }

    //int value_length = subseq_length - KEY_SIZE;

    if(subseq_length == 58){
        for(int i = 0; i<VALUE_SIZE_26; i++){
            value_26[i] = data->subseq[i];
        }
    }
    else{
        for(int i = 0; i<VALUE_SIZE_24; i++){
            value_24[i] = data->subseq[i];
        }
    }

    if(subseq_length_flag)
        storeInIndexerMap_26(key, value_26 ,data->offset);
    else
        storeInIndexerMap_24(key, value_24 ,data->offset);

    //printIndexerMap_26();
    //printIndexerMap_24();
}
