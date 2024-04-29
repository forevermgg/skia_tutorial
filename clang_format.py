#!/usr/bin/env python3
import os
import sys
import argparse
import subprocess

path_list = ['src/glfw/',
             'src/opencv/',
             'src/sdl2/',
             'src/skia/50_percent/',
             'src/skia/alpha/',
             'src/skia/bitmap/',
             'src/skia/canvas/',
             'src/skia/drawable/',
             'src/skia/gpu/',
             'src/skia/test/',
             'src/skia/text/',
             'src/skia/use/',
             'src/skia/watermark/',
             'src/skia/']
suffix_tuple = ('.h', '.cpp', ".cc")

def main():
    parser = argparse.ArgumentParser(description='clang-format runner')
    parser.add_argument('-n', '--dry-run', action='store_true', help='If set, do not actually make the formatting changes')
    options = parser.parse_args()
    dry_run = '-n ' if options.dry_run else ''

    for path in path_list:
        for filename in sorted(os.listdir(path)):
            f = os.path.join(path, filename)
            if os.path.isfile(f) and f.endswith(suffix_tuple):
                cmd = 'clang-format ' + dry_run + '--Werror -i -style=file ' + f
                ret = subprocess.run(cmd, shell=True, universal_newlines=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                retcode = ('' if ret.returncode == 0 else (' >>> return code: ' + str(ret.returncode)))
                print('>>> ' + cmd + retcode)
                print(ret.stdout, end='')

if __name__ == '__main__':
    sys.exit(main())