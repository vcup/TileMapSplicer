from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps


class TileMapSplicer(ConanFile):
    name = "TileMapSplicer"
    version = "0.0.0"
    settings = "os", "compiler", "build_type", "arch"
    requires = "libvips/8.14.2", "argtable3/3.2.2"

    def generate(self):
        toolchain = CMakeToolchain(self)
        toolchain.generator = "Ninja"
        toolchain.generate()

        deps = CMakeDeps(self)
        deps.generate()
