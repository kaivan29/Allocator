FILES :=                              \
    .travis.yml                       \
    allocator-tests/nap754-TestAllocator.c++ \
    allocator-tests/nap754-TestAllocator.out \
    Allocator.h                         \
    Allocator.log                       \
    html                              \
    TestAllocator.c++                   \
    TestAllocator.out

CXX        := g++-4.8 -g
CXXFLAGS   := -pedantic -std=c++11 -Wall
LDFLAGS    := -lgtest -lgtest_main -pthread
GCOV       := gcov-4.8
GCOVFLAGS  := -fprofile-arcs -ftest-coverage
VALGRIND   := valgrind

check:
	@not_found=0;                                 \
    for i in $(FILES);                            \
    do                                            \
        if [ -e $$i ];                            \
        then                                      \
            echo "$$i found";                     \
        else                                      \
            echo "$$i NOT FOUND";                 \
            not_found=`expr "$$not_found" + "1"`; \
        fi                                        \
    done;                                         \
    if [ $$not_found -ne 0 ];                     \
    then                                          \
        echo "$$not_found failures";              \
        exit 1;                                   \
    fi;                                           \
    echo "success";

clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f TestAllocator
	rm -f TestAllocator.tmp

config:
	git config -l

scrub:
	make clean
	rm -f  Allocator.log
	rm -rf Allocator-tests
	rm -rf html
	rm -rf latex

status:
	make clean
	@echo
	git branch
	git remote -v
	git status

test: TestAllocator.tmp

allocator-tests:
	git clone https://github.com/cs371p-fall-2015/allocator-tests.git

html: Doxyfile Allocator.h TestAllocator.c++
	doxygen Doxyfile

log:
	git log > Allocator.log

Doxyfile:
	doxygen -g

# RunAllocator: Allocator.h Allocator.c++
# 	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) Allocator.c++ -o RunAllocator

# RunAllocator.tmp: RunAllocator
# 	./RunAllocator < RunAllocator.in > RunAllocator.tmp
# 	diff RunAllocator.tmp RunAllocator.out

TestAllocator: Allocator.h TestAllocator.c++
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) TestAllocator.c++ -o TestAllocator $(LDFLAGS)

TestAllocator.tmp: TestAllocator
	$(VALGRIND) ./TestAllocator                                       >  TestAllocator.tmp 2>&1
	$(GCOV) -b TestAllocator.c++ | grep -A 5 "File 'TestAllocator.c++'" >> TestAllocator.tmp
	cat TestAllocator.tmp
