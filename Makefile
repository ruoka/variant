CXX = clang

CXXFLAGS = -std=c++1z -nostdlib -L/usr/lib -L/usr/local/lib -lc++ -lSystem -Iinclude

# Make does not offer a recursive wildcard function, so here's one:
rwildcard = $(wildcard $1$2)$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

UNITTESTS_SRC = $(call rwildcard,test/,*.cpp)

UNITTESTS_BIN = $(patsubst %.cpp,%,$(UNITTESTS_SRC))

$(UNITTESTS_BIN) : $(@:test/%=test/%.cpp)
	$(CXX) $(CXXFLAGS) $(@:test/%=test/%.cpp) -o $@

example: example.cpp include/__variant.hpp include/__variant_storage.hpp
	$(CXX) $(CXXFLAGS) example.cpp -o example

.PHONY: all
all: $(UNITTESTS_BIN)

.PHONY: clean
clean:
	@rm -f example
	@rm -f $(UNITTESTS_BIN)

.PHONY: dump
dump:
#	@echo $(UNITTESTS_SRC)
	@echo $(UNITTESTS_BIN)
	@echo $(UNITTESTS_BIN:test/%=test/%.cpp)

.PHONY: test
test:
	$(foreach TEST, $(UNITTESTS_BIN), $(TEST);)
