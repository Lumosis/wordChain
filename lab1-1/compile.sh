 cd ./src
 g++ -fPIC -c WordChain.cpp -std=c++11
 g++ -shared -o libtest.so WordChain.o
 mv libtest.so ../GUI
 cd ../GUI
 pyinstaller -w -F gui.py 
 mv dist/gui .