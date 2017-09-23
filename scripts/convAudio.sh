#!/bin/bash
cd "all_audio"
for inp in *
do
    ffmpeg -i "$inp" -ac 1 -ar 16000 -sample_fmt s16 "../flac_audio/${inp/%m4a/flac}" > "/dev/null" 2>&1
done
