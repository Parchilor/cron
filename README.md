# Scheduled Task

The code in this repository is a scheduled task. That is personal view which in software architecture also immature.

# Support
The Scheduled Task is written in C and should be supported by the most UNIX systems. It have libcjson.so and the standar libraries in UNIX. As of 02/15-2017 it has been compiled and tested on the following operating systems:

* Ubuntu 14.04
* Ubuntu 16.04
 
BEWARE! It does not work in Windows

# How to use
To compile the code, simply type: make  
If in Ubuntu 14.04. Should modify the file config.linux.mk:
* CXX = $(CROSS_CONPILER)g++ TO CXX = $(CROSS_CONPILER)gcc
* LIBRARY_NAMES = cjson #crypto TO  LIBRARY_NAMES = cjson m #crypto  

If in Ubuntu 16.04. It can be g++ and without m.(I don't know why.)  

In \<TOPDIR\>/Debug/bin :
* 'main' is Scheduled Task.
* 'modify' is to change the cron list.

BEWARE! modify \<year\> \<mon\> \<day\> \<hour\> \<min\> \<sec\> \<User define name\>  
BEWARE! I have not used sec.I think min it's enough.But modify should add it.
# The Cron List
It use json to save the details in \<TOPDIR\>/files/cmd.  
Such as {"year":2017,"month":2,"day":15,"hour":10,"minute":0,"second":0,"comment":"Test1","tag":0}  
'Tag' is to differentiate the same comment
