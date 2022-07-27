PROGRAM_NAME = lhs

all : $(PROGRAM_NAME)

$(PROGRAM_NAME) : 
	@mkdir build ;\
	cd build ;\
	cmake ..
	$(MAKE) -C build

run: $(PROGRAM_NAME)
	./build/$(PROGRAM_NAME) 0.0.0.0 2207

test: $(PROGRAM_NAME)
	cd build ;\
	ctest .

again: $(PROGRAM_NAME) 
	cd build ;\
	ctest . --rerun-failed --output-on-failure

clean : 
	rm -rf build; \
