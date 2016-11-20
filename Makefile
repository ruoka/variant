CXX = /usr/local/bin/clang

CXXFLAGS = -std=c++1z -lc++ -lSystem -L/usr/local/lib -L/usr/lib -Iinclude -ferror-limit=1

# Make does not offer a recursive wildcard function, so here's one:
rwildcard = $(wildcard $1$2)$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

UNITTESTS_SRC = $(call rwildcard,test/,*.cpp)

UNITTESTS_BIN = $(patsubst %.cpp,%,$(UNITTESTS_SRC))

$(UNITTESTS_BIN) : $(@:test/%=test/%.cpp) include/experimental/variant
	$(CXX) $(CXXFLAGS) $(@:test/%=test/%.cpp) -o $@

example: example.cpp include/experimental/variant
	$(CXX) $(CXXFLAGS) example.cpp -o example

.PHONY: all
all: example $(UNITTESTS_BIN)

.PHONY: clean
clean:
	@rm -f example
	@rm -f $(UNITTESTS_BIN)

.PHONY: test
test: $(UNITTESTS_BIN)
	$(foreach TEST, $(UNITTESTS_BIN), $(TEST);)

.PHONY: dump
dump:
	@echo $(UNITTESTS_BIN)
	@echo $(UNITTESTS_BIN:test/%=test/%.cpp)
