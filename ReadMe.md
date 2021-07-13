# A toolbox base on cxx20

## CMAKE not supported module yet, so we have to use MSVC now

### requirement:
> linux: gcc version: gcc version 11.0.1 (experimental) (GCC) 

> windows: MSVC 19

### install/update gcc11

#### Linux/Unix
```bash
git clone https://github.com/gcc-mirror/gcc.git
cd gcc
./contrib/download_prerequisites
mkdir build && cd build
../configure --disable-multilib
make -j${cat /proc/cpuinfo | grep "processor" | wc -l}
sudo make install

# optional
cd ../..
rm -rf gcc
```

#### possible problem
> /usr/lib64/libstdc++.so.6: version GLIBCXX_3.4.29 not found
```bash
sudo mv /usr/lib/x86_64-linux-gnu/libstdc++.so.6 /usr/lib/x86_64-linux-gnu/libstdc++.so.6.bak

sudo ln -s /usr/local/lib64/libstdc++.so.6 /usr/lib/x86_64-linux-gnu/libstdc++.so.6
# command below
sudo cp /usr/local/lib64/libstdc++.so.6 /usr/lib/x86_64-linux-gnu/libstdc++.so.6 
# also work for me, recommend to use ln, only need to choose one way
```
