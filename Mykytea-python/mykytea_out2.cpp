// mykytea.cpp
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "mykytea.hpp"

#define ALLOCATE_SITE 246
int split(char ***ary, char* s, const char *delimiter){

  char *tmp = strdup(s);

  int i,n;
  for( i =n =0; (tmp=strtok(tmp, delimiter)) != NULL; i++){
    if (i >= n) {
      n += ALLOCATE_SITE;
      *ary = (char**)realloc(*ary, sizeof(char*)*n);
    }
    (*ary)[i] = strdup(tmp);
    tmp = NULL;
  }
  return i;
}



Mykytea::Mykytea(char* line)
{
  char delimiter_1[] = " ";
  unsigned int split_len1, j;
  char **ary1 = (char**)malloc( sizeof(char*) );
  split_len1 = split(&ary1, line, delimiter_1);

  /*
  const char* configs[MAX_LEN];
  int len = split_argv(str, configs);
  //cout << len << endl;
  //for(int i = 0; i < len; i++)
  // cout << configs[i] << endl;
  */
  printf("read_modelfile:%s\n", (const char*)ary1[0]);
  
  KyteaConfig* _config = new KyteaConfig();
  //_config->parseRunCommandLine(len, configs);
  _config->parseRunCommandLine(1, (const char**)ary1[0]);
  config = _config;
  
  Kytea* _kytea = new Kytea(_config);
  //_kytea->readModel(_config->getModelFile().c_str());
  _kytea->readModel(ary1[0]);
  kytea = _kytea;
  util = _kytea->getStringUtil();
  
}

Mykytea::~Mykytea()
{
  if(kytea != NULL) delete kytea;
}

vector<string>* Mykytea::getWS(string str){
    vector<string>* vec = new vector<string>;
    KyteaString surface_string = util->mapString(str);
    KyteaSentence sentence(surface_string, util->normalize(surface_string));
    kytea->calculateWS(sentence);

    const KyteaSentence::Words & words =  sentence.words;

    for(int i = 0; i < (int)words.size(); i++) {
         (*vec).push_back(util->showString(words[i].surface));
    }
    return vec;
}

vector<Tags>* Mykytea::getTags(string str){
    vector<Tags>* ret_words = new vector<Tags>;

    KyteaString surface_string = util->mapString(str);
    KyteaSentence sentence(surface_string, util->normalize(surface_string));
    kytea->calculateWS(sentence);

    for(int i = 0; i < config->getNumTags(); i++)
        kytea->calculateTags(sentence,i);

    const KyteaSentence::Words & words =  sentence.words;

    for(int i = 0; i < (int)words.size(); i++) {
        tags vec_tag;
        for(int j = 0; j < (int)words[i].tags.size(); j++) {
            vector< pair<string, double> > vec_tmp;
            for(int k = 0; k < 1; k++) {
                vec_tmp.push_back( make_pair(util->showString(words[i].tags[j][k].first), words[i].tags[j][k].second) );
            }
            vec_tag.push_back( vec_tmp );
        }
        struct Tags t = { util->showString(words[i].surface), vec_tag };
        (*ret_words).push_back( t );
    }
    return ret_words;
}

vector<Tags>* Mykytea::getAllTags(string str){
    vector<Tags>* ret_words = new vector<Tags>;

    KyteaString surface_string = util->mapString(str);
    KyteaSentence sentence(surface_string, util->normalize(surface_string));
    kytea->calculateWS(sentence);

    for(int i = 0; i < config->getNumTags(); i++)
        kytea->calculateTags(sentence,i);

    const KyteaSentence::Words & words =  sentence.words;

    for(int i = 0; i < (int)words.size(); i++) {
        tags vec_tag;
        for(int j = 0; j < (int)words[i].tags.size(); j++) {
            vector< pair<string, double> > vec_tmp;
            for(int k = 0; k < (int)words[i].tags[j].size(); k++) {
                vec_tmp.push_back( make_pair(util->showString(words[i].tags[j][k].first), words[i].tags[j][k].second) );
            }
            vec_tag.push_back( vec_tmp );
        }
        struct Tags t = { util->showString(words[i].surface), vec_tag };
        (*ret_words).push_back( t );
    }
    return ret_words;
}

string Mykytea::getTagsToString(string str)
{
    KyteaString surface_string = util->mapString(str);
    KyteaSentence sentence(surface_string, util->normalize(surface_string));
    kytea->calculateWS(sentence);

    for(int i = 0; i < config->getNumTags(); i++)
        kytea->calculateTags(sentence,i);

    const KyteaSentence::Words & words =  sentence.words;

    string ret_str;
    for(int i = 0; i < (int)words.size(); i++) {
        ret_str += util->showString(words[i].surface);
        for(int j = 0; j < (int)words[i].tags.size(); j++) {
            for(int k = 0; k < 1; k++) {
                ret_str += "/";
                ret_str += util->showString(words[i].tags[j][k].first);
            }
        }
        ret_str += " ";
    }
    return ret_str;
  
}
