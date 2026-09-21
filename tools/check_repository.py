"""Check committed/staged Git blobs without requiring UE or downloading LFS assets."""
import re
import subprocess
import sys
from pathlib import PurePosixPath

LFS_EXTENSIONS = {
    '.uasset', '.umap', '.ubulk', '.uexp', '.fbx', '.blend', '.psd', '.psb',
    '.kra', '.clip', '.max', '.ma', '.mb', '.spp', '.sbsar', '.tga', '.png',
    '.jpg', '.jpeg', '.tif', '.tiff', '.exr', '.hdr', '.dds', '.wav', '.ogg',
    '.mp3', '.flac', '.mp4', '.mov', '.glb',
}
GENERATED = {'binaries', 'intermediate', 'saved', 'deriveddatacache', '.vs', '__pycache__', 'packagedbuilds'}
POINTER = re.compile(rb'version https://git-lfs.github.com/spec/v1\n(?:ext-[^\n]+\n)*oid sha256:[a-f0-9]{64}\nsize [0-9]+\n?\Z')

def git(*args):
    return subprocess.check_output(['git', *args])

def main():
    entries = git('ls-files', '--stage', '-z').split(b'\0')
    errors = []
    checked = 0
    for entry in entries:
        if not entry:
            continue
        metadata, name = entry.split(b'\t', 1)
        mode, oid, stage = metadata.decode('ascii').split()
        path = name.decode('utf-8', errors='replace')
        parts = PurePosixPath(path)
        checked += 1
        if stage != '0':
            errors.append(f'{path}: unresolved merge conflict')
            continue
        if mode not in {'100644', '100755'}:
            continue
        if any(p.lower() in GENERATED for p in parts.parts[:-1]):
            errors.append(f'{path}: generated/cache directory must not be committed')
        size = int(git('cat-file', '-s', oid))
        if parts.suffix.lower() in LFS_EXTENSIONS:
            if size > 1024 or not POINTER.fullmatch(git('cat-file', 'blob', oid)):
                errors.append(f'{path}: expected Git LFS pointer; install LFS and re-add this file')
        elif size > 10 * 1024 * 1024:
            errors.append(f'{path}: ordinary Git blob exceeds project 10 MiB policy; discuss LFS or release storage')
    for error in errors:
        print(f'ERROR: {error}')
    print(f'Checked {checked} indexed files; {len(errors)} error(s). UE build/assets still require manual verification.')
    return bool(errors)

if __name__ == '__main__':
    sys.exit(main())
