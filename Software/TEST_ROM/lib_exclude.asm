;
; lib_exclude.asm is included by HuC as the very first
; file to be included. This is intended to be used for
; whatever compile-time variable overrides are necessary
; 
;  DISABLEJOYSCAN will disable the automatic scanning of
;  joypads during the VBlank period 60 times per second
; 
DISABLEJOYSCAN        .equ    1

