all: clean copy run

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

run:
	@echo "Running 'dvhop-example'..."
	cd ~/ns-allinone-3.30.1/ns-3.30.1 && \
	./waf --run dvhop-example
