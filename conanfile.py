from conans import ConanFile, CMake, tools


class MatplotlibcppConan(ConanFile):
    name = "matplotlibcpp"
    version = "0.1"
    license = "MIT"
    author = "Cyrus Liu (cyrus.liu@aptiv.com)"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Matplotlibcpp here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "CMakeLists.txt", "cmake/*", "src/*"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_EXAMPLES"] = False
        cmake.configure()
        cmake.build()

#Explicit way:
#self.run('cmake %s/hello %s'
#%(self.source_folder, cmake.command_line))
#self.run("cmake --build . %s" % cmake.build_config)
    def package(self):
        cmake = CMake(self)
        cmake.install()

    # def package_info(self):
    #     self.cpp_info.libs = ["hello"]
