#!/bin/bash
# Copy all the files in folder $1 to folder $2, and also replace ' ' with _
cd ~/Music/iTunes/iTunes\ Media/Voice\ Memos
for f in *
do
    cp "$f" ~/Desktop/vml/build/all_audio/"${f// /_}"
done
