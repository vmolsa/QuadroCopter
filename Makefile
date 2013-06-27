all:
	make -C 3dparty
	make -C src

clean:
	make -C 3dparty clean
	make -C src clean
