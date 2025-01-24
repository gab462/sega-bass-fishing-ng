LIBS := ./external/DPP/build/install/lib/libdpp.so ./libolive.a ./libmsf_gif.a
SRC := src/main.cpp $(wildcard src/commands/*.cpp)

main: $(LIBS) $(SRC)
	c++ -std=c++23 -Wall -Wextra -O2 \
		-I ./external/DPP/build/install/include/ -I ./external \
		$(SRC) -o main \
		-L . -l:libolive.a -l:libmsf_gif.a -L ./external/DPP/build/install/lib/ -ldpp \
		-Wl,-rpath ./external/DPP/build/install/lib/

./external/DPP/build/install/lib/libdpp.so:
	cmake -S ./external/DPP/ -B ./external/DPP/build -DCMAKE_INSTALL_PREFIX=./external/DPP/build/install
	cmake --build ./external/DPP/build -j16
	make -C ./external/DPP/build install

./libolive.a:
	cc -std=c99 -O2 -DOLIVEC_IMPLEMENTATION -DOLIVECDEF="" -c ./external/olive.c -o ./olive.o
	ar rcs ./libolive.a ./olive.o

./libmsf_gif.a:
	cc -std=c99 -O2 -x c -DMSF_GIF_IMPL -c ./external/msf_gif.h -o ./msf_gif.o
	ar rcs ./libmsf_gif.a ./msf_gif.o
