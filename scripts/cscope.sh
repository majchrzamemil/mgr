#!/bin/bash
find /home/emil/Documents/MGR/mgr -name '.c' -o -name '.cc' -o -name *.cpp -o -name '.h' -o -name '*.hpp' > /home/emil/Documents/MGR/mgr/cscope.files

cd /home/emil/Documents/MGR/mgr
cscope -b
ctags -L cscope.files
