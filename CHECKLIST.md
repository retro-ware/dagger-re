# Daggerfall byte-perfect decomp: checklist

Base: brotchie/daggerfall-decomp (copied in, no .git). Originals: gog/ (read-only), orig/1.07.213/FALL.EXE.

## Done
- [x] Extract GOG installer to gog/ (innoextract)
- [x] FALL.EXE SHA-1 c49a2ceb… = brotchie's target 1.07.213
- [x] Toolchain: Watcom C/C++32 10.0a (game C), TASM 4.0 (XnGine, object 2), CauseWay 3.32 stub
- [x] venv + requirements; find_functions.py reproduces 3315 fns / 98.4% of object 1

## Compilers
- [x] Watcom C/C++ 10.0a → third_party/watcom10/w10a/WATCOM/ (from the user's DOS-installer run; package: ~/OldCompilers/Watcom-10.0a)
- [x] Borland TASM 4.0 (CMD16.PAK, CMDLINE.PAK) → third_party/tasm/BIN/

## Next
- [x] tools/build-and-verify.sh → build/FALL.EXE: OK (SHA-1 c49a2ceb, 2297/2297 game fns, 719/719 XnGine fns)
- [ ] Libraries: fingerprint Watcom RTL against LIB386 (the ~1000 non-game fns, incl. MemCheck)
- [ ] Full link with WLINK instead of splicing into the original image (data object 3, fixups, CauseWay stub)
- [ ] Build with nothing original

## Other executables
- DAGGER.EXE: skipped (user: out of scope)
- SETUP.EXE, arena2/REPORT.EXE: Watcom C32 + CauseWay
- FIXSAVE.EXE: CauseWay, Bethesda 1996-97 (compiler version still to pin)
- data/DAGAUTO.EXE: MSVC Windows autorun, out of scope
