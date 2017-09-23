#!/bin/bash
# Converts the json response into concatenated plaintext file
cd "json_response"
for file in *
do
    grep "transcript" $file | cut -c26- | rev | cut -c2- | tr '"' " " | rev | tr -d '\n' > "../plaintext/${file/response/text}"
    echo "" >> "../plaintext/${file/response/text}"
done
cd ..
