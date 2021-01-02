#ifndef LISTDIRECTORY_H
#define LISTDIRECTORY_H

#include "canonical.h"
#include "headers.h"
#include "command.h"
using namespace std;

void cursor_move_normal(vector<string>, int );
void commandMode(vector<string>,int);

/********************* Display the metadata of the file/directory passed as parameter ***********************/
void display(string str)
{
    struct stat inode;
    double size;
    char mtime[50],temp[20];
    string perm,uid,gid;
    time_t s_time;
    struct tm local_time;
    vector<string> name=split(str, "/");
        
        stat((str).c_str(),&inode); 
        perm="";
        perm+=(S_ISDIR(inode.st_mode) ? "d" : "-");
        perm+=((inode.st_mode & S_IRUSR) ? "r" : "-");
	      perm+=((inode.st_mode & S_IWUSR) ? "w" : "-");
	      perm+=((inode.st_mode & S_IXUSR) ? "x" : "-");
	      perm+=((inode.st_mode & S_IRGRP) ? "r" : "-");
	      perm+=((inode.st_mode & S_IWGRP) ? "w" : "-");
	      perm+=((inode.st_mode & S_IXGRP) ? "x" : "-");
          perm+=((inode.st_mode & S_IROTH) ? "r" : "-");
	      perm+=((inode.st_mode & S_IWOTH) ? "w" : "-");
	      perm+=((inode.st_mode & S_IXOTH) ? "x" : "-");
        
        printf("%s",perm.c_str());
        size=inode.st_size;
        if(size>100)
        {
          size=size/1024;
          if(size>100)
            {
              size=size/1024;
              printf("%8.2fM\t",size);
            }
          else 
            printf("%8.2fK\t",size);
                
            
        }
        else
            printf("%8.2f \t",size);
          
            
        uid=getpwuid(inode.st_uid)->pw_name;   
        gid=getgrgid(inode.st_gid)->gr_name;
        printf("%s\t",uid.c_str());
        printf("%s\t",gid.c_str());
        strcpy(mtime,ctime(&inode.st_mtime));
        s_time = inode.st_mtime;
        localtime_r(&s_time, &local_time);
        strftime(mtime, sizeof mtime, "%b %d %H:%M", &local_time);
        
        printf("%10s\t",mtime);
        cout<<name[name.size()-1]<<endl;

}

/********************* Display the contents of the directory ***********************/
vector<string> listDirectory(const char *dirname,int w)
{
    DIR *dir;
    struct dirent *sd;
    vector<string> v;
    

    //open the directory
    dir=opendir(dirname);
    if(dir==NULL)
    {
        printf("error in opening directory\n");
        exit(1);
    }

    //Loop through each file in the directory and it to the list
    while((sd=readdir(dir))!=NULL)
       { 
         string s=sd->d_name,s1=dirname;
         s=s1+'/'+s;
         v.push_back(s);
       }
    closedir(dir);    
    
    //sort the files in alphabetical order
    sort(v.begin(),v.end());
    cur_dir=v[0].substr(0,v[0].length()-2) ;
   
    //clear the screen before printing the content;
    printf("\33c");

    int n = (w<v.size())?w:v.size();
    for(auto i=0;i<n;i++)
    {
        display(v[i]);
    }
    if(cmd_mode)
        commandMode(v,w);
    else 
        cursor_move_normal(v,w);
    return v;
}

void openFile(const char *filename)   //converting string into const char* is allowed
{
    if (fork() == 0) {
		execl("/usr/bin/xdg-open", "xdg-open", filename, (char *)0);
		exit(1);
	}
}

/********************************* Normal Mode ******************************************/
void cursor_move_normal(vector<string> list,int w)
{
    int c,pos=0,n,k=-1,l=-1;
    n=(w<list.size())?w:list.size();
    struct stat inode;
    set_pos(n+3,0);
    printf("*******************************NORMAL MODE*************************");
    set_pos(0,0);
    string homedir = home;
    while(1)
    {
        c =0;

        switch((c=getch())) {
        case 65:if(pos>0){
                    printf("\033[1A");
                    pos--;
                }

            break;
        case 66:if(pos<n-1){
                    printf("\033[1B") ;
                    pos++;
                }
            break;
        case 68:
            if(!back_stack.empty())
            {
               string str=back_stack.top();
               back_stack.pop();
               front_stack.push(cur_dir);
                listDirectory(str.c_str(),w);
            }
            
            break;
        case 67:
            if(!front_stack.empty())
            {
                string str=front_stack.top();
                front_stack.pop();
                back_stack.push(cur_dir);
                listDirectory(str.c_str(),w);
            }
            
            break;
        case 10:
             pos+=(k>0)?k:0;
             if(pos!=0)
             {
                 stat((list[pos]).c_str(),&inode);
                if(S_ISDIR(inode.st_mode))
                    {
                        back_stack.push(cur_dir);
                        while(!front_stack.empty())
                            front_stack.pop();
                        if(pos==1)
                            displayParent_dir(list[0],w);    
                        else
                            listDirectory((list[pos]).c_str(),w);
                        
                    }
                else 
                    openFile((list[pos]).c_str());
             }
             break;
        case 127:
              while(!front_stack.empty())
                front_stack.pop();
              back_stack.push(cur_dir);
              listDirectory((list[1]).c_str(),w);  

              break;     
        case 104:
              while(!front_stack.empty())
                front_stack.pop();
              back_stack.push(cur_dir);
              listDirectory(homedir.c_str(),w);  
              break;  
        case 107:  if(k==-1 && l==-1 && w+1<list.size())
                     {  k=1;l=w+1;}
                    
                    if(l<list.size())
                    {
                        k++;l++;
                        printf("\33c");
                        for(int i=k;i<l;i++)
                            display(list[i]);
                        set_pos(n+3,0);
                        printf("*******************************NORMAL MODE*************************");    
                        set_pos(w,0);
                        pos=w;    
                    }
                   break;       
        case 108: if(k>0 && l<=list.size())
                  {
                      k--;l--;
                      printf("\33c");
                        for(int i=k;i<l;i++)
                            display(list[i]);
                      set_pos(n+3,0);
                      printf("*******************************NORMAL MODE*************************");      
                      set_pos(0,0); 
                      pos=0;
                  }  
                  break;         
        case 81:
        case 113:
            set_pos(n+3,0);eraseline;
            exit(1);
        case 58:
            commandMode(list,w);
        
        
        }

    }
}

/********************************* Command Mode ************************************/
void commandMode(vector<string> list,int w)
{
   int n=(w<list.size())?w+3:list.size()+3;
   int y=0;
   cmd_mode=1;
   setCommand(n,&y);
   set_pos(n,y);
   char c=0;
   char a,b;
   string cmd="";
   vector<string> command;
   while(1)
   {
       switch((c=getch()))
       {   
           case 27: cmd_mode=0;set_pos(n+3,0);clearfrom;
                    cursor_move_normal(list,w);
           case 10: y=0;
                    setCommand(n,&y);
                    command=split(cmd," ");
                    cmd="";
                    if(command[0].compare("copy")==0)
                        copy_cmd(command);
                    else if(command[0].compare("search")==0){
                        result.clear();
                        search_cmd(cur_dir,command);
                        show_result();
                    }
                    else if(command[0].compare("move")==0)
                        move_cmd(command);
                    else if(command[0].compare("rename")==0)
                        rename_cmd(command);
                    else if(command[0].compare("create_file")==0)
                        createFile_cmd(command);
                    else if(command[0].compare("create_dir")==0)
                        createDir_cmd(command);
                    else if(command[0].compare("delete_file")==0)
                        deleteFile_cmd(command);
                    else if(command[0].compare("delete_dir")==0)
                        deleteDir_cmd(command);
                    else if(command[0].compare("goto")==0)
                        goto_cmd(command);
                    break;
           case 127: if(y>14)
                        set_pos(n,--y);
                        printf("\x1b[0K");
                        cmd=cmd.substr(0,cmd.length()-1);
                        break;
           default: cout<<c; set_pos(n,++y); cmd+=c;
       }
   }
}

#endif