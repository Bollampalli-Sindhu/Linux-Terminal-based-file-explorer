#ifndef canonical_H
#define canonical_H
#include "headers.h"
#include "listdirectory.h"
#include "command.h"
using namespace std;

#define esc 27
#define set_pos(x,y) printf("%c[%d;%dH",esc,x,y)
#define eraseline printf("%c[%dK",esc,2);
#define clearfrom printf("\e[0J");
#define create_dir(directory, m) return mkdir(directory, m)

/* utility function to take the input in canonical mode. */

int getch(void)
{
 int ch;
 struct termios oldtio;
 struct termios newtio;
 tcgetattr(0, &oldtio); //0 is file discriptor for standard input
 
 newtio = oldtio; 
 newtio.c_lflag &= ~(ICANON | ECHO);   //switch to noncanonical and nonecho mode 
 tcsetattr(0, TCSANOW, &newtio);

 ch=cin.get();                         //Take the input
                   
 if(tcsetattr(0, TCSANOW, &oldtio))    //switch to cananical mode
    {
        fprintf(stderr,"Set attributes failed");
        exit(1);
    }
 
    return ch; 
}
vector<string> split(string s, string delim)
{
    string token;
    size_t loc=0;
    vector<string>res;
    while ((loc = s.find(delim)) != string::npos) {
        token = s.substr(0, loc);
        res.push_back(token);
        s.erase(0, loc + delim.length());
    }
    res.push_back(s);
    return res;
}
void setCommand(int n, int *y)
{
   set_pos(n,*y);
   clearfrom;
   printf("command mode:"); 
   *y=14;
}
string absolute_path(string path)
{
    string abs_path = "";
    if (path[0] == '~') 
        abs_path = string(home) + path.substr(1, path.length());
   
    else if (path[0] == '/') 
        abs_path = string(home) + path;
    
    else if (path[0] == '.' && path[1] == '/') 
        abs_path = cur_dir + path.substr(1, path.length());
    
    else 
        abs_path = cur_dir + "/" + path;
    
    return abs_path;
}
int isDir(string path)
{
    struct stat inode;
    if (stat(path.c_str(), &inode) != 0) {
        return 0;
    }
    if (S_ISDIR(inode.st_mode))
        return 1;
    else
        return 0;
}

mode_t get_mode(string src)
{
    struct stat inode;
    stat(src.c_str(), &inode);
    mode_t m=0;
    
    
    
    m = m | ((inode.st_mode & S_IROTH)?0004:0);
    m = m | ((inode.st_mode & S_IWOTH)?0002:0);
    m = m | ((inode.st_mode & S_IXOTH)?0001:0);

    m = m | ((inode.st_mode & S_IRUSR)?0400:0);
    m = m | ((inode.st_mode & S_IWUSR)?0200:0);
    m = m | ((inode.st_mode & S_IXUSR)?0100:0);

    m = m | ((inode.st_mode & S_IRGRP)?0040:0);
    m = m | ((inode.st_mode & S_IWGRP)?0020:0);
    m = m | ((inode.st_mode & S_IXGRP)?0010:0);
    

    return m;
}
void displayParent_dir(string list,int w)
{
    vector<string> path=split(list,"/");
    string dir=path[0];
    for(int i=1;i<path.size()-2;i++)
        dir+="/"+path[i];
    listDirectory(dir.c_str(),w);    
}
string getpath(string path, string name)
{
    return path+"/" +name;
}

#endif