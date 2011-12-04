#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "hasher.h"

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
    cout << subStr << endl ;
    sendRefSeq(getBitString(subStr), offset, refId) ;
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
  char buf[50], buf1[1], bufL[51];
  long offset, foffset, lastEndLine;
  fstream fp(fileName, ios::in);
  memset(bufL, '\0', sizeof(bufL));
  fp.getline(bufL, 51);
  string chrom(bufL);
  offset = fp.tellg();
  memset(bufL, '\0', sizeof(bufL));
  fp.read(bufL, 50);
  foffset = offset;
  string str(bufL);
//  cout << foffset << " " << str << " " << chrom << endl;
  generateSubSequence(str, offset, chrom) ;
  while(!fp.eof()){
    offset = fp.tellg();
    fp.read(buf1, 1);
    if(buf1[0] != '\n'){
     offset = fp.tellg();
     string str1(buf1);
     str = str.substr(1) + str1;
     foffset++;
     if(foffset == lastEndLine)
	 foffset++;
     //     cout << foffset << " " << str << " " << chrom << endl;
     generateSubSequence(str, offset, chrom) ;
    }else{
      lastEndLine = offset;
      offset = fp.tellg();
      memset(bufL, '\0', sizeof(bufL));
      fp.getline(bufL, 51);
      string tmp(bufL);
      if(tmp[0] == '>'){
        chrom = tmp;
        offset = fp.tellg();
        foffset = offset;
        memset(bufL, '\0', sizeof(bufL));
        fp.read(bufL, 50);
        string str2(bufL);
        str = str2;
        if(str.length() == 50)
	    //          cout << foffset << " " << str << " " << chrom << endl;
	    generateSubSequence(str, offset, chrom) ;
	continue;
      }else if(tmp[0] == 'c' || tmp[0] == 'C' || tmp[0] == 'G' || tmp[0] == 'g' ||
                        tmp[0] == 'A' ||tmp[0] == 'a' ||tmp[0] == 'T' ||tmp[0] == 't' || tmp[0] == 'n' || tmp[0] == 'N' ){
	fp.seekg(offset);
	continue;
      }
    }
  }
  fp.close();
}

int main(int argc, char **argv){
  if(argc < 2){
    cout << "Wrong input : <fileName> <interface>" << endl;
    return 0;
  }
  init_sender(argv[2]) ;

  readFile(argv[1]);
  return 0;
}

