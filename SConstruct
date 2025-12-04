#!/usr/bin/env python
import os
import sys

from methods import print_error


libname = "godot-artnet"
projectdir = "demo"

localEnv = Environment(tools=["default"], PLATFORM="")

# Build profiles can be used to decrease compile times.
# You can either specify "disabled_classes", OR
# explicitly specify "enabled_classes" which disables all other classes.
# Modify the build_profile.json file as needed and uncomment the line below or
# manually specify the build_profile parameter when running SCons.

# localEnv["build_profile"] = "build_profile.json"

customs = ["custom.py"]
customs = [os.path.abspath(path) for path in customs]

opts = Variables(customs, ARGUMENTS)
opts.Update(localEnv)

Help(opts.GenerateHelpText(localEnv))

env = localEnv.Clone()

if not (os.path.isdir("godot-cpp") and os.listdir("godot-cpp")):
    print_error("""godot-cpp is not available within this folder, as Git submodules haven't been initialized.
Run the following command to download godot-cpp:

    git submodule update --init --recursive""")
    sys.exit(1)

if not (os.path.isdir("lib-artnet-4-cpp") and os.path.isdir("lib-artnet-4-cpp/artnet")):
    print_error("""lib-artnet-4-cpp is not available within this folder, as Git submodules haven't been initialized.
Run the following command to download lib-artnet-4-cpp:

    git submodule update --init --recursive""")
    sys.exit(1)

env = SConscript("godot-cpp/SConstruct", {"env": env, "customs": customs})

env.Append(CPPPATH=["src/", "lib-artnet-4-cpp/artnet"])

# Add compatibility include path for Windows builds
if env["platform"] == "windows":
    env.Append(CPPPATH=["src/compat"])

# Add artnet library sources
artnet_sources = [
    "lib-artnet-4-cpp/artnet/ArtNetController.cpp",
    "lib-artnet-4-cpp/artnet/utils.cpp",
]

# Add platform-specific network interface
if env["platform"] == "macos" or env["platform"] == "ios":
    artnet_sources.append("lib-artnet-4-cpp/artnet/network_interface_bsd.cpp")
elif env["platform"] == "linux" or env["platform"] == "android":
    artnet_sources.append("lib-artnet-4-cpp/artnet/network_interface_linux.cpp")
elif env["platform"] == "windows":
    # Windows might need special handling, but for now use BSD interface
    artnet_sources.append("lib-artnet-4-cpp/artnet/network_interface_bsd.cpp")
else:
    # Default to BSD interface for other platforms
    artnet_sources.append("lib-artnet-4-cpp/artnet/network_interface_bsd.cpp")

# Enable exceptions for artnet library sources (artnet library requires exceptions)
artnet_env = env.Clone()
if "CXXFLAGS" in artnet_env:
    # Remove -fno-exceptions if present and add -fexceptions
    artnet_env["CXXFLAGS"] = [flag for flag in artnet_env["CXXFLAGS"] if flag != "-fno-exceptions"]
artnet_env.Append(CXXFLAGS=["-fexceptions"])

# Force include header to fix missing cstring include in library
if "is_msvc" in artnet_env and artnet_env["is_msvc"]:
    # MSVC: use /FI (Force Include)
    artnet_env.Append(CCFLAGS=["/FI", "src/artnet_force_include.h"])
else:
    # GCC/Clang: use -include
    artnet_env.Append(CCFLAGS=["-include", "src/artnet_force_include.h"])

# Add Windows compatibility for artnet library
if env["platform"] == "windows":
    # Add compatibility include path (must be first to shadow system headers)
    artnet_env.Prepend(CPPPATH=["src/compat"])
    # Link with ws2_32 library for Windows sockets
    artnet_env.Append(LIBS=["ws2_32"])

# Add Linux compatibility for endian headers
if env["platform"] == "linux" or env["platform"] == "android":
    # Add compatibility include path for Linux endian headers
    artnet_env.Prepend(CPPPATH=["src/compat"])

# Compile artnet sources with exceptions enabled
artnet_objects = []
for source in artnet_sources:
    artnet_objects.append(artnet_env.SharedObject(source))

sources = Glob("src/*.cpp")
sources.extend(artnet_objects)

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

# .dev doesn't inhibit compatibility, so we don't need to key it.
# .universal just means "compatible with all relevant arches" so we don't need to key it.
suffix = env['suffix'].replace(".dev", "").replace(".universal", "")

lib_filename = "{}{}{}{}".format(env.subst('$SHLIBPREFIX'), libname, suffix, env.subst('$SHLIBSUFFIX'))

library = env.SharedLibrary(
    "bin/{}/{}".format(env['platform'], lib_filename),
    source=sources,
)

copy = env.Install("{}/bin/{}/".format(projectdir, env["platform"]), library)

default_args = [library, copy]
Default(*default_args)
