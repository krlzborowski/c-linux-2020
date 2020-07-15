all: compile link

compile: clean
	cd parasite && make
	cd provider && make

link: compile
	mkdir -p ODP
	ln -s ../provider/provider ODP/provider
	ln -s ../parasite/parasite ODP/parasite

clean:
	rm -rf ODP
	cd provider && make clean
	cd parasite && make clean