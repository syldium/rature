project('rature', 'c', default_options : ['c_std=c17', 'warning_level=3'])

mupdf = dependency('mupdf')

executable('rature', 'src/main.c', 'src/redact.c', dependencies : mupdf)
