from distutils.core import setup, Extension


with open("README.md", "r") as fh:
    long_description = fh.read()


module = Extension('acudp',
                    #  define_macros = [('MAJOR_VERSION', '1'),
                                     #  ('MINOR_VERSION', '0')],
                    #  include_dirs = ['/usr/local/include'],
                    #  libraries = ['tcl83'],
                    #  library_dirs = ['/usr/local/lib'],
                    sources = ['src/acudp_module.c', 
                               'src/acudp_type.c',
                               'src/handshake_response_type.c'])

setup (name = 'acudp',
       version = '0.1',
       description = 'AC UDP client module',
       long_description=long_description,
       author = 'Victor Picon',
       url = '',
       ext_modules = [module])
