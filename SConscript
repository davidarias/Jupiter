import os

def get_sources(src, ext):
    "get sources of ext extension from src dir"
    sources = []

    for root, dirnames, filenames in os.walk(src):
        for dirname in dirnames:
            sources += Glob(os.path.join(root, dirname, '*.%s' % ext))

    sources += Glob('%s/*.%s' % (src, ext))

    return sources

# scons -Q DEBUG=true
cppdefines = {}
for key, value in ARGLIST:
    cppdefines[key] = value

env = Environment(
    CPPDEFINES = cppdefines,
    CPPPATH=['./src'],
    CCFLAGS=['-Wall', '-Wextra', '-std=c++14', '-O3'],
    LIBS='mpdec'
)

sources = get_sources('src/compiler', 'cpp')
sources += get_sources('src/misc', 'cpp')
sources += get_sources('src/objects', 'cpp')
sources += get_sources('src/primitives', 'cpp')
sources += get_sources('src/utils', 'cpp')
sources += get_sources('src/vm', 'cpp')
sources += ['src/main.cpp']

env.Program(
    'jupiter',
    sources,
)
