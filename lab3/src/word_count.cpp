#include "word_count.h"

// implementation details
int word_count(const std::string& line, int start_idx) {

  // YOUR IMPLEMENTATION HERE

  int nwords = 0;
  bool previous_space = true; // start as a space
  for (int i=start_idx; i<line.length(); ++i) 
  {
    if (line[i] == ' ' || line[i] == '\'') //flag previous space
    {
      previous_space = true;
    }
    else if( previous_space && (line[i] >= 'A') )//check if after a space is a special character except for apostrophe
    {
      previous_space = false;
      nwords++;
    }
  }

  return nwords;
}