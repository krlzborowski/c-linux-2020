all: compile link

compile: clean
	# cd familiada && make
	cd parasite && make
	cd provider && make

link: compile
	mkdir -p ODP
	ln -s ../provider/provider ODP/provider
	ln -s ../parasite/parasite ODP/parasite
	ln -s ../familiada/familiada ODP/familiada

clean:
	rm -rf ODP
	cd provider && make clean
	cd parasite && make clean
	# cd familiada && make clean