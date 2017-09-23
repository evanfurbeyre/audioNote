#!/bin/bash
# Send audio in1 minute chunks
apiKey="key_goes_here"

cd flac_audio
for f in *; do
    # Get the length of audio clip
    dur=$(ffprobe -i "$f" -show_entries format=duration -v quiet -of csv="p=0")

    if [[ $(echo "$dur < 59" | bc) -ne 0 ]]; then     # If duration is a minute or less
        FILENAME="request-"`date +"%s".json`
        cat <<EOF > $FILENAME
        {
            "config": {
            "encoding":"FLAC",
            "sample_rate":16000,
            "languageCode": "en-US"
            },
            "audio": {
                "content":
            }
        }
EOF
        # Encode audio into base64 format
        echo \"`base64 $f`\" > audio.base64
        sed -i '' -e '/"content":/r audio.base64' $FILENAME

        # Call the speech API (requires an API key) and put output in json_response folder
        curl -s -X POST -H "Content-Type: application/json" --data-binary @${FILENAME} https://speech.googleapis.com/v1beta1/speech:syncrecognize?key=$apiKey > ../json_response/response${f/.flac/}

        rm $FILENAME
    else
        FILENAME="request-"`date +"%s".json`
        # Split file into 59 second chunks in temp dir
        idx=0
        count=0
        mkdir tempAudio
        mkdir tempJson

        # Split the audio into tempAudio
        while [[ $(echo "$dur > $idx" | bc) -ne 0 ]]; do
            sox "$f" "tempAudio/`printf %02d $count`_$f" trim "$idx" 59 2>/dev/null
            idx=$(expr $idx + 59)
            count=$(expr $count + 1)
        done
        cd tempAudio

        # Send each chunk
        for chunk in *; do
            cat <<EOF > $FILENAME
            {
                "config": {
                "encoding":"FLAC",
                "sample_rate":16000,
                "languageCode": "en-US"
                },
                "audio": {
                    "content":
                }
            }
EOF
            echo \"`base64 $chunk`\" > ../audio.base64
            sed -i '' -e '/"content":/r ../audio.base64' $FILENAME

            # Call the speech API (requires an API key), put responses in tempJson
            curl -s -X POST -H "Content-Type: application/json" --data-binary @${FILENAME} https://speech.googleapis.com/v1beta1/speech:syncrecognize?key=$apiKey > ../tempJson/response${chunk/.flac/}
            rm $FILENAME
        done

        # Concat the json chunks back together
        cd ../tempJson
        for j in *; do
            cat "$j" >> ../../json_response/response${f/.flac/}
        done
        cd ..
        rm -r tempAudio
        rm -r tempJson
    fi
done
cd ..
