# coding: utf-8

from cx_Freeze import setup, Executable

executables = [Executable('OSC categories.py',
               base='Win32GUI')]

options = {
    'build_exe': {
        'include_msvcr': True,
    }
}

setup(name='seeOSC.py',
      version='0.0.2',
      description='Programm for work with .osc files!',
      executables=executables,
      options=options)
