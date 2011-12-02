#include <stdio.h>
#include <iostream>
#include <fstream>
#include <bitset>
#include <string.h>

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
string getBitString(string str){
  cout << str << endl;
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
  cout << bitStream << endl;
  bitset<58> bits(bitStream);
  return bits.to_string();
}

void generateSubSequence(string str, long offset){
  for(int i=0; i<7; ++i){
    string subStr = "";
    for(int j=0; j<50; j++){
      if(slides[i][j] == '1')
	subStr += str[j];
    }
    string s = getBitString(subStr);
    cout << s << endl;
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
    generateSubSequence(str, offset);    
  }
  fp.close();
}

int main(int argc, char **argv){
  if(argc < 2){
    cout << "Wrong input : <output.exe> <fileName>" << endl;
    return 0;
  }
  getReadFromFile(argv[1]);

  return 0;
}

