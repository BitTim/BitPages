vpath %.cc src

PRODUCT := BitPresent
PRODUCT2 := BitPresentEditor

CXX := g++
CXXFLAGS := -c -std=c++17 `wx-config-gtk3 --cxxflags`
CXXLIBS := -lSDL2 -lSDL2_ttf -lSDL2_image -lhpdf `wx-config-gtk3 --libs`
CXXINCDIRS := -I.

SOURCE := $(shell find src/ -name *.cc)
OBJECTS := $(patsubst %.cc,.cache/%.o,$(SOURCE))
DEPFILES := $(patsubst %.cc,.cache/%.d,$(SOURCE))

NUMOPS := $(shell echo $(OBJECTS) | wc -w)
OPS := 0

all: $(PRODUCT) $(PRODUCT2)

$(PRODUCT): $(OBJECTS)
	$(eval OPS=$(shell echo $$(($(OPS)-$(OPS)))))
	@echo -e "Linking CXX objects to: \033[0;33m$@\033[0m"
	@$(CXX) $(CXXLIBS) $^ -o $@

.cache/src/%.d: src/%.cc
	$(eval OPS=$(shell echo $$(($(OPS)+1))))
	@echo -e "\033[0;34m[$(OPS)/$(NUMOPS)]\033[0m Generating CXX dependency: \033[0;33m$@\033[0m"
	@$(CXX) $(CXXINCDIRS) -MM $< > $@

.cache/src/%.o: src/%.cc
	$(eval OPS=$(shell echo $$(($(OPS)+1))))
	@echo -e "\033[0;32m[$(OPS)/$(NUMOPS)]\033[0m Building CXX object: \033[0;33m$@\033[0m"
	@$(CXX) $(CXXFLAGS) $(CXXINCDIRS) -o $@ $<

init:
	@mkdir -p .cache/src
	@rsync -a -f"+ */" -f"- *" src/ .cache/src/

delDirs:
	@rm -R .cache
	@rm $(PRODUCT)

clean: delDirs init

run: $(PRODUCT)
	@./$<

debug: $(PRODUCT)
	@gdb $<

-include $(DEPFILES)
