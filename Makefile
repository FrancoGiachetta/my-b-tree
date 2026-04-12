BUILD_DIR=build

.PHONY: build
build:
	cmake -S . -B ${BUILD_DIR}
	cmake --build ${BUILD_DIR}

.PHONY: clean
clean:
	rm -rf ${BUILD_DIR}
	rm compile_commands.json
