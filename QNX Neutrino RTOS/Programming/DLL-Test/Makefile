all: src/DLL1.cpp src/DLL2.cpp src/App.cpp
	ntox86-g++-3.3.5 -fPIC -c -o libDLL1.o src/DLL1.cpp -I./include
	ntox86-g++-3.3.5 -fPIC -c -o libDLL2.o src/DLL2.cpp -I./include
	ntox86-g++-3.3.5 -shared -Wl,-soname,libDLL1.so -o libDLL1.so libDLL1.o
	ntox86-g++-3.3.5 -shared -Wl,-soname,libDLL2.so -o libDLL2.so libDLL2.o -L. -lDLL1
	ntox86-g++-3.3.5 -o App.o -c src/App.cpp -I./include
	ntox86-g++-3.3.5 -o App -L. -lDLL2 App.o
	ntox86-objdump -x libDLL1.so > libDLL1.txt
	ntox86-objdump -x libDLL2.so > libDLL2.txt
	
clean:
	rm libDLL1.o
	rm libDLL2.o
	rm libDLL1.so
	rm libDLL2.so
	rm App.o
	rm App
	rm libDLL1.txt
	rm libDLL2.txt