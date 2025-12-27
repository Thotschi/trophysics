# color codes
GREEN := \033[0;32m
NC := \033[0m # No Color

DEBUG := True

debug_build_cython:
# @ sign suppresses command output
	@echo "> ${GREEN}Make sure to have an activated virtual environment with cython, numpy and setuptools installed.${NC}"
	@echo "> ${GREEN}Otherwise build process will fail.${NC}"
	@echo "> ${GREEN}Also note that virtual environments must not be commited to git because they depend on local system.${NC}"

	@echo "> ${GREEN}Deleting old build...${NC}"
# delete old shared library
	make clean_build
# setting sys-argv[1] = --debug=$(DEBUG) allows for passing system arguments to python when executing setup.py
# that way we can use "make build_cython DEBUG=True" to set compiler_directives dedicated for debugging to True
	@echo "> ${GREEN}Building Cython files...${NC}"
	python3 setup.py --debug=$(DEBUG) build_ext --inplace
	@echo "> ${GREEN}Build process completed. You may now deactivate virtual environment.${NC}"

test_cython:
	@echo "> Make sure"
	fish tests/test_cython.fish

install:
	pip install .

clean_build:
	rm -rf build/
	rm -rf dist/
	rm -rf trophysics.egg-info
	rm -f trophysics/integrate/_cython/_core.c
	rm -f trophysics/integrate/_cython/*.html
	rm -f trophysics/integrate/_cython/*.so
	rm -f *.so
