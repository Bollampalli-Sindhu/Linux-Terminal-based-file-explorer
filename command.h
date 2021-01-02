#ifndef COMMAND_H
#define COMMAND_H
#include "headers.h"
#include "listdirectory.h"
#include "canonical.h"
vector<string> result;

/********************** Function to copy the file *****************************/
void copy_file(string src, string dest)
{
    int fd_src, fd_dest;
    mode_t m = get_mode(src);
    char ch;
    if((fd_src=open(src.c_str(),O_RDONLY))==-1){
        printf("Error in Opening source file");
        return;
    }
    if((fd_dest=open(dest.c_str(),O_CREAT|O_WRONLY,m))==-1){
        printf("file already exists");
        return;
    }
    while(read(fd_src,&ch,1))
        write(fd_dest,&ch,1);
        
       
    close(fd_src);
    close(fd_dest);
}

/********************** Function to copy the directory *****************************/
void  copy_directory(string src, string dest)
{
    DIR *dir;
    struct dirent* sd;
    string s,d;
    dir=opendir(src.c_str());
    if(dir==NULL)
    {
        printf("error in opening directory\n");
        exit(1);
    }
    while((sd=readdir(dir))!=NULL){
        if(!strcmp(sd->d_name,".")||!strcmp(sd->d_name,".."))
            continue;
        d=sd->d_name;
        s=src+"/"+d; d=dest+"/"+d;
        if(!isDir(s))
            copy_file(s,d);
        else{
            mode_t m=get_mode(s);
            if(mkdir(d.c_str(),m)==-1){
                printf("Couldn't create directory");
                return;
            }
            else 
                copy_directory(s,d);
        }
        
    }

}

void copy_cmd(vector<string> cmd)
{
    if(cmd.size()<3)
        printf("Invalid Command");
    else {
        string dest_folder=absolute_path(cmd[cmd.size()-1]);
        if(!(isDir(dest_folder)))
         {
             printf("Invalid destination");
             return ;
         }
        for(int i=1;i<cmd.size()-1;i++)
        {
            string src=absolute_path(cmd[i]);
            vector<string> names=split(cmd[i],"/");
            string dest=dest_folder+"/"+names[names.size()-1];
            if(isDir(src)){
                
                mode_t m= get_mode(src);
                if(mkdir(dest.c_str(),m)==-1){
                    printf("couldn't create directory");
                    return;
                }
                copy_directory(src,dest);
                
            }
            else 
                copy_file(src,dest);
                
        }
    }
}
/********************** Search for file or directory recursively from the current directory *****************************/
void search_cmd(string path, vector<string> cmd)
{
    if(cmd.size()!=2)
        printf("Invalid Command");
    else {
    DIR *dir;
    struct dirent *sd;
    string src = getpath(path,cmd[1]);
    dir=opendir(path.c_str());
    if(dir==NULL)
        printf("error in opening directory: %s",path.c_str());
    while((sd=readdir(dir))!=NULL)
    { 
        if(!strcmp(sd->d_name,".")||!strcmp(sd->d_name,".."))
            continue;
        string dest=getpath(path,sd->d_name);
        if(!strcmp(src.c_str(),dest.c_str()))
            result.push_back(dest);
        if(isDir(dest))
            search_cmd(dest,cmd);
    }    
    
    }

}
void show_result()
{
    if(result.size())
        cout<<"True";
    else 
        cout<<"False";
}

/********************** Function to move the file/directory *****************************/
void move_cmd(vector<string> cmd)
{
    if(cmd.size()<3)
        printf("Invalid Command");
    else {
        copy_cmd(cmd);
        for(int i=1;i<cmd.size()-1;i++)
        {
            string src=absolute_path(cmd[i]);
            if(isDir(src))
            {   
                deletedirectory(src);
                if(rmdir(src.c_str())==-1)
                    printf("Could't move directory");
                else 
                    listDirectory(cur_dir.c_str(),rows-3);
            }
            else if(unlink(src.c_str())==-1)
                cout<<"Couldn't move the file";
            else 
               listDirectory(cur_dir.c_str(),rows-3); 
        }
        
    }
}

/********************** Function to rename the file/directory *****************************/
void rename_cmd(vector<string> cmd)
{
    if(cmd.size()!=3)
        printf("Invalid Command");
    else {
        string src=absolute_path(cmd[1]);
        string dest=absolute_path(cmd[2]);
        vector<string> s=split(src,"/");
        vector<string> destinatn=split(dest,"/");
        s[s.size()-1]=destinatn[destinatn.size()-1];
        string d=s[0];
        for(int i=1;i<s.size();i++)
            d+="/"+s[i];
        if(rename(src.c_str(),d.c_str())==-1)
            cout<<"couldn't rename the file/ directory";
        else
            listDirectory(cur_dir.c_str(),rows-3);
    }
    
}

/********************** Function to create the file *****************************/
void createFile_cmd(vector<string> cmd)
{
    if(cmd.size()!=3)
        printf("Invalid Command");
    else {
        string dest_folder=absolute_path(cmd[2]);
        string file=cmd[1];
        string dest_file=dest_folder+"/"+file;
        if(creat(dest_file.c_str(),0600)==-1)
            printf("couldn't create file");
        else 
            listDirectory(cur_dir.c_str(),rows-3);
    }
    
}

/********************** Function to create the directory *****************************/
void createDir_cmd(vector<string> cmd)
{
    if(cmd.size()!=3)
        printf("Invalid Command");
    else {
        string dest_folder=absolute_path(cmd[2]);
        string folder=cmd[1];
        string dest=dest_folder+"/"+folder;
        if(mkdir(dest.c_str(),0755)==-1)
            printf("couldn't create directory");
        else 
            listDirectory(cur_dir.c_str(),rows-3);
    }
}

/********************** Function to delete the file *****************************/
void deleteFile_cmd(vector<string> cmd)
{
    if(cmd.size()!=2)
        printf("Invalid Command");
    else {
        string src=absolute_path(cmd[1]);
        if(unlink(src.c_str())==-1)
            printf("couldn't delete file");
        else 
            listDirectory(cur_dir.c_str(),rows-3);
    }
}

/********************** Function to delete the directory *****************************/
void deleteDir_cmd(vector<string> cmd)
{
    if(cmd.size()!=2)
        printf("Invalid Command");
    else {
        string src=absolute_path(cmd[1]);
        if(isDir(src)){
            deletedirectory(src);
            if(rmdir(src.c_str())==-1)
                printf("Couldn't delete directory");
            else
               listDirectory(cur_dir.c_str(),rows-3);
        }
    }
}

/********************** Function to goto to specified directory *****************************/
void goto_cmd(vector<string> cmd)
{
    if(cmd.size()!=2)
        printf("Invalid Command");
    else {
        string src=absolute_path(cmd[1]);
        vector<string> path=split(src,"/");
        if(path[path.size()-1]==".")
            return;
        back_stack.push(cur_dir);
        if(path[path.size()-1]=="..")
            displayParent_dir(src.substr(0,src.length()-1),rows-3);
        else
            listDirectory(src.c_str(),rows-3);
    }
}

/********************** delete directory utility function *****************************/
void deletedirectory(string src)
{
    DIR *dir;
    struct dirent *sd;
    string s,d;
    dir=opendir(src.c_str());
    if(dir==NULL)
        printf("error in opening directory: %s",src.c_str());
    while((sd=readdir(dir))!=NULL)
    {
        if(!strcmp(sd->d_name,".")||!strcmp(sd->d_name,".."))
            continue;
        d=sd->d_name;
        s=src+"/"+d; 
        if(!isDir(s))
        {
            if(unlink(s.c_str())==-1)
            { 
                printf("couldn't delete file %s ",src.c_str());
                return;
            }
        }
        else{
            deletedirectory(s);
            if(rmdir(s.c_str())==-1)
                printf("cant delte %s", s.c_str());
        }  
    }
}


#endif