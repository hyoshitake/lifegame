#-------------------------------------------------------
# BCC Developer 1.2.18
# Copyright (C) 2003 jun_miura@hi-ho.ne.jp
#-------------------------------------------------------
.autodepend
CC=bcc32
RC=brc32
CFLAG=-W  -3 -Od -w- -AT -pc -H- -k -b -v -y -L..\..\�R���p�C��\DX_BCC\DxLib_BCC\�v���W�F�N�g�ɒǉ����ׂ��t�@�C��_BCC�p -DDEBUG
OUTDIR=-nDebug
CINCS=-I..\..\�R���p�C��\DX_BCC\DxLib_BCC\�v���W�F�N�g�ɒǉ����ׂ��t�@�C��_BCC�p
TARGET=Debug\lifegame.exe
SRC1=F:\Programing\�\�[�X�R�[�h\lifegame\main.cpp
OBJ1=Debug\main.obj

TARGET: $(TARGET)

$(TARGET): $(OBJ1)
    $(CC) $(CFLAG) -e$(TARGET) $(OBJ1)

$(OBJ1): $(SRC1)
    $(CC) $(CFLAG) $(OUTDIR) $(CINCS) -c $(SRC1)
