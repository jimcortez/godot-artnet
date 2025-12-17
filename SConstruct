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

# On Windows, add compat directory to regular env so our code can find compatibility headers
# when including library headers (library headers need netinet/in.h etc. on Windows)
# This is only needed on Windows where POSIX headers don't exist
if env["platform"] == "windows":
    compat_path = os.path.abspath("src/compat")
    if "is_msvc" in env and env["is_msvc"]:
        env.Append(CCFLAGS=["/I", compat_path])
    else:
        env.Append(CCFLAGS=["-isystem", compat_path])

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
    # On Windows, the library code includes network_interface_linux.h and instantiates NetworkInterfaceLinux
    # So we need to compile network_interface_linux.cpp on Windows too
    # Our compatibility headers make it work on Windows
    artnet_sources.append("lib-artnet-4-cpp/artnet/network_interface_linux.cpp")
else:
    # Default to BSD interface for other platforms
    artnet_sources.append("lib-artnet-4-cpp/artnet/network_interface_bsd.cpp")

# Enable exceptions for artnet library sources (artnet library requires exceptions)
artnet_env = env.Clone()
if "CXXFLAGS" in artnet_env:
    # Remove -fno-exceptions if present and add -fexceptions
    artnet_env["CXXFLAGS"] = [flag for flag in artnet_env["CXXFLAGS"] if flag != "-fno-exceptions"]
artnet_env.Append(CXXFLAGS=["-fexceptions"])

# For MSVC, enable exception handling with /EHsc
if "is_msvc" in artnet_env and artnet_env["is_msvc"]:
    # /EHsc enables C++ exception handling (synchronous exception model)
    artnet_env.Append(CCFLAGS=["/EHsc"])

# For web/wasm builds, configure Emscripten exceptions properly
# Emscripten C++ exceptions conflict with setjmp/longjmp, so we need to disable
# the longjmp mechanism when exceptions are enabled
if env["platform"] == "web":
    # Remove SUPPORT_LONGJMP flag as it conflicts with C++ exceptions
    # Emscripten will use its own exception mechanism instead
    if "CCFLAGS" in artnet_env:
        artnet_env["CCFLAGS"] = [flag for flag in artnet_env["CCFLAGS"] if not flag.startswith("-sSUPPORT_LONGJMP")]
    if "LINKFLAGS" in artnet_env:
        artnet_env["LINKFLAGS"] = [flag for flag in artnet_env["LINKFLAGS"] if not flag.startswith("-sSUPPORT_LONGJMP")]
    # Enable Emscripten C++ exceptions
    artnet_env.Append(CCFLAGS=["-sDISABLE_EXCEPTION_THROWING=0"])
    artnet_env.Append(LINKFLAGS=["-sDISABLE_EXCEPTION_THROWING=0"])

# Force include header to fix missing cstring include in library
# Get absolute path to the force-include header
force_include_path = os.path.abspath("src/artnet_force_include.h")
if "is_msvc" in artnet_env and artnet_env["is_msvc"]:
    # MSVC: use /FI (Force Include) - requires absolute path on Windows
    artnet_env.Append(CCFLAGS=["/FI", force_include_path])
else:
    # GCC/Clang: use -include
    artnet_env.Append(CCFLAGS=["-include", "src/artnet_force_include.h"])

# Add compatibility include path scoped to artnet library sources only
# The compat headers provide:
# - Windows: POSIX networking headers via WinSock2
# - Linux/Android: sys/_endian.h compatibility (macOS-specific header the library includes)
# - Web/Emscripten: networking function implementations (inet_ntop, inet_addr, etc.)
# macOS and iOS have all required headers natively, so they don't need compat path
# (Using compat path on macOS/iOS causes #include_next issues with cross-compilation)
if env["platform"] != "macos" and env["platform"] != "ios":
    compat_path = os.path.abspath("src/compat")
    if "is_msvc" in artnet_env and artnet_env["is_msvc"]:
        # MSVC: use /I (Include Path) - system includes are treated specially
        artnet_env.Append(CCFLAGS=["/I", compat_path])
    else:
        # GCC/Clang: use -isystem to mark as system header directory
        # This suppresses warnings and keeps it scoped to artnet sources
        artnet_env.Append(CCFLAGS=["-isystem", compat_path])

# Add Windows-specific settings
if env["platform"] == "windows":
    # Link with ws2_32 library for Windows sockets
    artnet_env.Append(LIBS=["ws2_32"])

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

# Link ws2_32 on Windows for the final library (needed for Windows socket functions)
if env["platform"] == "windows":
    env.Append(LIBS=["ws2_32"])

library = env.SharedLibrary(
    "bin/{}/{}".format(env['platform'], lib_filename),
    source=sources,
)

copy = env.Install("{}/bin/{}/".format(projectdir, env["platform"]), library)

default_args = [library, copy]
Default(*default_args)
