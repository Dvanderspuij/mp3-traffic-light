# Pre cleanup
rm -rf *.tar.gz*
rm -rf libraries

# Create folder and download libraries
mkdir libraries
wget https://github.com/adafruit/Adafruit_NeoPixel/archive/v1.0.6.tar.gz
tar -x -f v1.0.6.tar.gz -C libraries/
wget https://github.com/madsci1016/Sparkfun-MP3-Player-Shield-Arduino-Library/archive/1.02.02.tar.gz
tar -x -f 1.02.02.tar.gz -C libraries/

# Copy libraries to arduino folder.
cp -R libraries/Adafruit_NeoPixel-1.0.6 ~/Documents/Arduino/libraries/Adafruit_NeoPixel
cp -R libraries/Sparkfun-MP3-Player-Shield-Arduino-Library-1.02.02/SFEMP3Shield ~/Documents/Arduino/libraries/SFEMP3Shield
cp -R libraries/Sparkfun-MP3-Player-Shield-Arduino-Library-1.02.02/SdFat ~/Documents/Arduino/libraries/SdFat

# Post cleanup
rm -rf *.tar.gz*
