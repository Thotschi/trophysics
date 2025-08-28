cd /usr/lib

echo "> Removing old shared library in /usr/lib..."
sudo rm -f libtlib.so
echo "> Done."

# copy new shared library into /usr/lib where gcc and system can find it
echo "> Copying new shared library into /usr/lib..."
sudo cp PATH/t-repo/headers/libtlib.so /usr/lib
echo "> Done."

# change owner and group of the shared library: 
# 0 --> no special permission
# 7 --> full permissions (read, write, execute) for the owner of the file
# 5 --> read and execute permissions for the group
# 5 --> read and execute permissions for other users
echo "> Changing owner and group of the shared library..."
sudo chmod 0755 /usr/lib/libtlib.so
echo "> Done."

# update shared library cache
echo "> Updating cache..."
sudo ldconfig
echo "> Done."