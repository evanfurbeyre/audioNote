#!/bin/bash
# Print all the files in folder plaintext
cd "plaintext"
for f in *
do
    echo "Confirm note:"
    echo "-------------------------"
    cat "$f"
    # confirm the printout or edit it
    echo "-------------------------"
    while true; do
        read -p "Press c to continue, e to edit, or d to delete: " cmnd 
        case $cmnd in
            [Cc]* ) echo "Continuing..."; break;;
            [Ee]* ) vi "$f"; break;;
            [Dd]* ) echo "Deleting..."; rm "$f"; break;;
            * ) echo 'Invalid Answer, choose again';;
        esac
    done    
done
cd ..
