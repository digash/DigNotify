@echo off
REM Copyright (c) 2006-2008 Dimitry Gashinsky. All rights reserved.
REM
REM Author: Dimitry Gashinsky <dimitry@gashinsky.com>
REM Id: 9132cb34-25f1-4f56-9029-4671049d7e29

IF NOT DEFINED EMACSHOME set EMACSHOME=H:\opt\emacs-22.1
IF NOT DEFINED EMACS_SERVER_FILE set EMACS_SERVER_FILE=Z:\.emacs.d\server\%COMPUTERNAME%\server
IF DEFINED ALTERNATE_EDITOR set ALTERNATE_EDITOR=

"%EMACSHOME%\bin\emacsclientw.exe" --no-wait --eval "(dig:notify \"%*%\")"
