#!/bin/bash

start_time=`date +%s`

clear

# ./build/の削除と作成

# directory="./build/"

# if [ -d "$directory" ]; then
#    echo "directory:" "$directory found"
#    rm -rf "$directory"
#    echo "directory:" "$directory" " was deleted."

#    mkdir "$directory"
#    echo "directory:" "$directory" " was created."

# else
#    mkdir  "$directory"
# fi

# ファイルの除去(.txtはCMaleLists.txtも消してしまってまずい)
# find ./ -name "*.csv" -type f -delete
# find ./ -name "*.png" -type f -delete
# find ./ -name "*.mp4" -type f -delete

# tree > tree.txt

# ninjaでbuildする
cmake -B ./build -G Ninja
cd ./build
ninja -t clean
ninja
cd ..

# ./build/main

# /usr/bin/python3.11 ./python/mkpng.py

# /usr/bin/ffmpeg -r 10 -i ./ey_pngs/ey_timestep_%06d.png -c:v libx264 -pix_fmt yuv420p ./mp4/output.mp4

# vlc ./mp4/output.mp4

end_time=`date +%s`

time=$((end_time - start_time))

printf "(bash:) processing time=%s [sec]\n" $time
