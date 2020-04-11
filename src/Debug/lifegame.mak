#-------------------------------------------------------
# BCC Developer 1.2.18
# Copyright (C) 2003 jun_miura@hi-ho.ne.jp
#-------------------------------------------------------
.autodepend
CC=bcc32
RC=brc32
CFLAG=-W  -3 -Od -w- -AT -pc -H- -k -b -v -y -L..\..\コンパイラ\DX_BCC\DxLib_BCC\プロジェクトに追加すべきファイル_BCC用 -DDEBUG
OUTDIR=-nDebug
CINCS=-I..\..\コンパイラ\DX_BCC\DxLib_BCC\プロジェクトに追加すべきファイル_BCC用
TARGET=Debug\lifegame.exe
SRC1=F:\Programing\ソースコード\lifegame\main.cpp
OBJ1=Debug\main.obj

TARGET: $(TARGET)

$(TARGET): $(OBJ1)
    $(CC) $(CFLAG) -e$(TARGET) $(OBJ1)

$(OBJ1): $(SRC1)
    $(CC) $(CFLAG) $(OUTDIR) $(CINCS) -c $(SRC1)
