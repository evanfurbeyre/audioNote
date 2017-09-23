#!/bin/bash
# Moves all the files in folder $1 to folder $2
cd "$1"
for f in *
do
    mv "$f" "../$2/$f"
done
