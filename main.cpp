#include "headers.h"
#include "listdirectory.h"

int main()
{
  getcwd(home, sizeof(home));
  vector<string> v;
  ioctl(STDIN_FILENO, TIOCGWINSZ, &win);
  rows=win.ws_row;
  v= listDirectory(home,rows-3); 
  
  return 0;
}