#ifndef HEADERS_H
#define HEADERS_H
#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<time.h>
#include<string.h>
#include<termios.h>
#include<iostream>
#include<pwd.h>
#include<grp.h>
#include<set>
#include<bits/stdc++.h>
#include <sys/ioctl.h>
#include<sys/wait.h>
using namespace std;
char home[PATH_MAX];
stack<string> back_stack,front_stack;
struct winsize win;
int rows;
int cmd_mode;
string cur_dir;
int getch(void);
vector<string> split(string , string);
string absolute_path(string);
int isDir(string);
mode_t get_mode(string);
vector <string> listDirectory(const char*,int);
void displayParent_dir(string ,int );
string getpath(string, string);
void deletedirectory(string);
#endif