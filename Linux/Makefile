vpath %.cc src

PRODUCT := bitPresent

CXX := g++
CXXFLAGS := -c `wx-config-gtk3 --cxxflags`
CXXLIBS := -lSDL2 -lSDL2_ttf -lSDL2_image -lhpdf `wx-config-gtk3 --libs`
CXXINCDIRS := -I.

SOURCE := $(shell find . -name *.cc)
OBJECTS := $(patsubst %.cc,.cache/%.o,$(SOURCE))
DEPFILES := $(patsubst %.cc,.cache/%.d,$(SOURCE))

NUMOPS := $(shell echo $(OBJECTS) | wc -w)
OPS := 0

$(PRODUCT): $(OBJECTS)
	@echo -e "Linking CXX objects to: \033[0;33m$@\033[0m"
	@$(CXX) $(CXXLIBS) $^ -o $@

.cache/%.o: %.cc
	$(eval OPS=$(shell echo $$(($(OPS)+1))))
	@echo -e "\033[0;32m[$(OPS)/$(NUMOPS)]\033[0m Building CXX object: \033[0;33m$@\033[0m"
	@$(CXX) $(CXXFLAGS) $(CXXINCDIRS) -o $@ $<

.cache/%.d: %.cc
	$(eval OPS=$(shell echo $$(($(OPS)+1))))
	@echo -e "\033[0;34m[$(OPS)/$(NUMOPS)]\033[0m Generating CXX dependency: \033[0;33m$@\033[0m"
	@$(CXX) $(CXXINCDIRS) -MM $< > $@

init:
	@mkdir -p .cache/src
	@rsync -a -f"+ */" -f"- *" src/ .cache/src/

delDirs:
	@rm -R .cache

clean: delDirs init

run: $(PRODUCT)
	@./$<

debug: $(PRODUCT)
	@gdb $<

-include $(DEPFILES)
