#!/bin/sh


rm -f /home/victor/Disertation/Workplace/bin/ld
ln -s /usr/bin/ld.gold /home/victor/Disertation/Workplace/bin/ld
#ln -s /home/victor/Disertation/Workplace/llvm-bin/bin/ld.lld /home/victor/Disertation/Workplace/bin/ld

export PATH=/home/victor/Disertation/Workplace/bin:$PATH
