#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "hasher.h"
#include "fileIO.h"

using namespace std;

string slides[] = {
{"111*1**111*1**111*1**111*1**111*1**111*1**111*1**1"},
{"*111*1**111*1**111*1**111*1**111*1**111*1**111*1**"},
{"**111*1**111*1**111*1**111*1**111*1**111*1**111*1*"},
{"1**111*1**111*1**111*1**111*1**111*1**111*1**111*1"},
{"*1**111*1**111*1**111*1**111*1**111*1**111*1**111*"},
{"1*1**111*1**111*1**111*1**111*1**111*1**111*1**111"},
{"11*1**111*1**111*1**111*1**111*1**111*1**111*1**11"}
};

// A 00 G 01 T 10 C 11
bitset<SUBSEQ_SIZE> getBitString(string str){
  int len = str.length();
  string bitStream = "";
  for( int i=0; i<len; ++i){
    if(str[i] == 'a' || str[i] == 'A'|| str[i] == 'n'|| str[i] == 'N')
      bitStream += "00";
    else if(str[i] == 'g'|| str[i] == 'G')
      bitStream += "01";
    else if(str[i] == 't'|| str[i] == 'T')
      bitStream += "10";
    else if(str[i] == 'c'|| str[i] == 'C')
      bitStream += "11";
  }
  bitset<SUBSEQ_SIZE> bits(bitStream);
  if(len == 29)
      bits.set(MSB_SUBSEQ) ;

  return bits;
}

void generateSubSequence(string str, long offset, string refId){
  for(int i=0; i<7; ++i){
    string subStr = "";
    for(int j=0; j<50; j++){
      if(slides[i][j] == '1')
	subStr += str[j];
    }
    cout << str << endl;
//    sendRefSeq(getBitString(subStr), offset, refId) ;
  }
}

// This fun is just reading 1 line at a time.
void getReadFromFile(char *fileName){
  char buf[51];
  long offset;
  fstream fp(fileName,ios::in);
  while(!fp.eof()){
    memset(buf,'\0',51);
    offset = fp.tellg();
    fp.getline(buf,51);
    string str(buf);
    if(str.length() != 50)
      continue;
//    generateSubSequence(str, offset);    
  }
  fp.close();
}

// Instead of the 3 cout statements, make a function call
// the function should take 3 parameters offset, 50 character string, string with >chrmX
int readFile(char *fileName){
  string buf, chrom, tmpStr;
  long offset = 0, foffset, lastEndLine=0;
  char ch;

  //read the header of the file
  buf.assign((const char *)fileMap, offset, 6);
  chrom = buf;  

  offset +=7;
  foffset = offset;
  
  //read 50 bytes from the reference file
  buf.assign((const char *)fileMap, offset, 50);
//  cout << foffset << " " << buf << " " << chrom << endl;
  generateSubSequence(buf, foffset, chrom) ;

  offset += 50;

  while(fileMap[offset] != '\0'){
    ch = fileMap[offset];
    offset++;
    if(ch != '\n'){
      buf = buf.substr(1) + ch;
      foffset++;
      if(foffset == lastEndLine)
	    foffset++;
//      cout << foffset << " " << buf << " " << chrom << endl;
      generateSubSequence(buf, foffset, chrom) ;
    }else{
      lastEndLine = offset-1;
      
      tmpStr.assign((const char *)fileMap, offset, 6);
      if(tmpStr[0] == '>'){
        chrom = tmpStr;
        offset += 7;
        foffset = offset;

        buf.assign((const char *)fileMap, offset, 50);
        if(buf.length() == 50){
//          cout << foffset << " " << buf << " " << chrom << endl;
	      generateSubSequence(buf, foffset, chrom) ;
	    }
        continue;
      }else if(buf[0] == 'c' || buf[0] == 'C' || buf[0] == 'G' || buf[0] == 'g' ||
                        buf[0] == 'A' ||buf[0] == 'a' ||buf[0] == 'T' ||buf[0] == 't' || buf[0] == 'n' || buf[0] == 'N' ){
	    continue;
      }
    }
  }
}

int main(int argc, char **argv){
  if(argc < 2){
    cout << "Wrong input : <fileName> <interface>" << endl;
    return 0;
  }
  init_sender(argv[2]) ;

  //This loads FILE to MEMORY ARRAY...'fileMap'
//  loadFileToMMap((unsigned char *)argv[1]);
  loadFullFile(argv[1]);
  readFile(argv[1]);
  return 0;
}

