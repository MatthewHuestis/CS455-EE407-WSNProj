all:
	@echo "Available options:"
	@echo "clean - Clean old source files out of NS3 AIO dir"
	@echo "copy - Copy current source files to NS3 AIO dir"
	@echo "run - Compiles and run the simulation"
	@echo "configure - Configure DV-hop example - only needs to be ran once"
	@echo "pull_pcaps - Pull PCAP files from NS3 AIO dir into cache"
	@echo "clear_pcaps - Delete PCAP files NS3 AIO dir and local cache"
	@echo "merge_pcaps - Merge PCAP files in cache into one"


clean:
	@echo "Cleaning old source directory..."
	rm -r ~/ns-allinone-3.30.1/ns-3.30.1/src/dvhop

copy:
	@echo "Copying source code to NS3 directory..."
	cp -R ./dvhop ~/ns-allinone-3.30.1/ns-3.30.1/src

configure:
	@echo "Configuring DV-Hop..."
	cd ~/ns-allinone-3.30.1/ns-3.30.1 && \
	./waf configure --enable-examples

pull_pcaps:
	@echo "Pulling PCAP files from NS3 directory..."
	mkdir -p pcap_cache
	cp ~/ns-allinone-3.30.1/ns-3.30.1/*.pcap ./pcap_cache

clear_pcaps:
	@echo "Clearing PCAP files from NS3 directory..."
	rm ~/ns-allinone-3.30.1/ns-3.30.1/*.pcap
	@echo "Clearing PCAP files from cached directory"
	rm ./pcap_cache/*.pcap

merge_pcaps:
	mergecap -w merged.pcap ./pcap_cache/*.pcap

run:
	@echo "Running 'dvhop-example'..."
	cd ~/ns-allinone-3.30.1/ns-3.30.1 && \
	./waf --run dvhop-example
