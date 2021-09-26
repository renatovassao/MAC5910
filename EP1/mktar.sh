#!/bin/bash

TARDEST=ep1-renato_matsuda_vassao_dias

make clean
rm -fr $TARDEST
mkdir $TARDEST
cp -r {src,bin,lib,Makefile,LEIAME,presentation.pdf} $TARDEST
tar czvf $TARDEST.tar.gz $TARDEST
rm -r $TARDEST