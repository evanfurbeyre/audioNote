Author: Evan Furbeyre
Version: 1.0
Date: 9/22/17

AudioNote:

AudioNote is a program that interprets and archives voice memos in searchable text files.

Prerequisites:
    -Mac and iPhone synced, with Voice Memo folder at ~/Music/iTunes/iTunes Media/Voice Memos
    -ffmpeg installed (audio/video converter program)
    -Your own API KEY you get through a free Google Cloud Platform account
    -Your API KEY copied into file sendAudio.sh, e.g.  apiKey="<your key>"
    -Basic knowledge of vim
    -Patience, this is my first project

Keyword interpretation:
-All commands start with "Control", which causes next word to be interpreted
-First order keywords ( after using one keyword, interpretation turns off )
    - Page: opens up a new page to contain a note
    - Done: closes the current page
    - Period: replaces with '.'
    - Comma: replaces with ','
    - Enter: replaces with '\n\n'
    - Tab: replaces with '\t'
-Second order keywords ( needs closing keyword to turn interpretation off )
    - Tag: Puts a '#' in front of the following words 
    - Stop: Turns off interpretation for second order keyword

Example Speech: "control page sandals made out of hot glue control tag invention idea stop control done"


How to Use:
-In the command line, navigate to main folder and type in 'make' to run Makefile
-Enter ./audioNote to run the program
    -Run:
    -Converts any audio in your Voice Memos folder to a note pag
    -Confirm pre-interpreted text files or edit with vim or delete
-Pages: Display all or search by one or more words 

Now, listen close for any passing thoughts you may want to keep track of!


