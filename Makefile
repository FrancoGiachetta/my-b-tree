BUILD_DIR=build

.PHONY: build
build:
	cmake -S . -B ${BUILD_DIR}
	cmake --build ${BUILD_DIR}

.PHONY: fmt
fmt:
	find . \
	-iname 'build' -prune \
	-o -type f \( -iname '*.h' -o -iname '*.cpp' -o -iname '*.hpp' \) -print \
	| clang-format --style=Microsoft -i --files=/dev/stdin

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}
	rm compile_commands.json
