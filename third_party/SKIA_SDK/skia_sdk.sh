# https://github.com/JetBrains/skia-pack/releases/
wget https://github.com/JetBrains/skia-pack/releases/download/m116-47d3027-1/Skia-m116-47d3027-1-macos-Release-arm64.zip
wget https://github.com/JetBrains/skia-pack/releases/download/m116-47d3027-1/Skia-m116-47d3027-1-macos-Release-x64.zip
for file in ./*.zip; do
    mkdir ${file%????}
    tar -xvf $file -C ./${file%????}
    rm $file
done