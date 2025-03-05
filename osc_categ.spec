# -*- mode: python ; coding: utf-8 -*-


a = Analysis(
    ['osc_categ.py'],
    pathex=[],
    binaries=[('./Aegis_osc.pyd', '.')
    ,('C:/Windows/System32/mfc140.dll', '.')
    , ('C:/Windows/System32/msvcp140.dll', '.')
    , ('C:/Windows/System32/vccorlib140.dll', '.')],
    datas=[('./model_24.12.24 description_24.12.20 ОСЦ ДЛЯ МАТВЕЯ ровно_copy_withotEarlyStop_100-epochs_32-batchsize.keras', '.')],
    hiddenimports=[],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    noarchive=False,
    optimize=0,
)
# Компоновка
pyz = PYZ(a.pure)

# Собираем .exe файл
exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.datas,
    [],
    name='OSC Categories',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=False,
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
)
