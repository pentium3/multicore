#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <curl/curl.h>
#include"tinyxml2.h" 

using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

vector<string> split(const string &s, const string &delimiter)
{
  vector<string> result;
  typedef string::size_type string_size;
  string_size i = 0;
  while(i != s.size()){
    int flag = 0;
    while(i != s.size() && flag == 0){
      flag = 1;
      for(string_size x = 0; x < delimiter.size(); ++x)
      {
              if(s[i] == delimiter[x]){
                  ++i;
                  flag = 0;
                  break;
              }
      }
    }
    flag = 0;
    string_size j = i;
    while(j != s.size() && flag == 0)
    {
      for(string_size x = 0; x < delimiter.size(); ++x)
      {
              if(s[j] == delimiter[x]){
                  flag = 1;
                  break;
              }
      }
      if(flag == 0)
          ++j;
    }
    if(i != j){
      result.push_back(s.substr(i, j-i));
      i = j;
    }
  }
  return result;
}

string RTrim(const string& str) 
{ 
  return str.substr(0,str.find_last_not_of("\n")); 
}

int main(void)
{
  CURL *curl;
  CURLcode res;
  string readBuffer;
  ifstream fin("data.txt");
  ifstream sin("stopwords.txt");
  string temp;
  string temp2;
  vector<string> url;
  vector<string> sw;
  while(getline(fin, temp))
  {
    //cout << temp << endl;
    url.push_back(temp);
  }
  while(getline(sin, temp2))
  {
    //cout << temp2 << endl;
    temp2 = RTrim(temp2);
    sw.push_back(temp2);
  }
  cout<<"TOTAL URL=="<<url.size()<<endl;

  for(int i = 0; i < url.size(); i++)
  {
    //cout << "2333" << endl;
    curl = curl_easy_init();
    if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url[i].c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    //printf("%s\n", readBuffer.c_str());
    }
    //printf("%s\n", readBuffer.c_str());
    //cout << readBuffer << endl;
    tinyxml2::XMLDocument doc;  
    doc.Parse(readBuffer.c_str());  
    readBuffer = "";
    tinyxml2::XMLElement *surface=doc.RootElement();
    const char * rootName = surface->Value();

    surface=surface->FirstChildElement(); 
    rootName = surface->Value();

    surface=surface->FirstChildElement(); 
    rootName = surface->Value();

    surface=surface->FirstChildElement(); 
    rootName = surface->Value();

    surface=surface->FirstChildElement(); 
    rootName = surface->GetText();  
    //cout << rootName << endl; 
    
    vector<string> contents;
    contents = split(rootName, " ,./[]()?!:\n");

    //for(int k = 0; k < sw.size(); k++)
      //cout << sw[k] << endl;

    //cout << contents[0] << endl << contents [1] << endl;

    for(int m = 0; m < contents.size(); m ++)
    {
      for(int j = 0; j < sw.size(); j++)
      {
        //cout<<contents[m]<<"---"<<sw[j]<<"---";
        transform(contents[m].begin(), contents[m].end(), contents[m].begin(), ::tolower);
        transform(sw[j].begin(), sw[j].end(), sw[j].begin(), ::tolower);
        if(contents[m] == sw[j])
        {
          //cout << contents[m] << endl;
          contents[m] = "";
          //cout<<"RRR"<<endl;
        }
        //cout<<endl;
      }
    }

    string s = "./data/" + to_string(i) + ".txt";
    ofstream out(s);
    out<<"###"<<to_string(i)<<".txt"<<endl;
    for(int k = 0; k < contents.size(); k++)
      if(contents[k]!="")
        out << contents[k] << " ";
    out<<endl;
    //out << rootName;
    out.close();
    cout<<s<<" Finished"<<endl;
  }
  
  return 0;
}