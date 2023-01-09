

Z_RAM_START         equ   $00000080     ; Die RAM beginnt immer bei $0080 und endet - je nach
Z_RAM_END           equ   $000000FF     ; Größe des Controllers - bei $047F (DZ16), D087F (DZ32)$
E_RAM_START         equ   $00000100
E_RAM_END           equ   $0000107F

ROM_DZ60_START      equ   $00001900     ; Hier beginnt die FLASH beim DZ60
ROM_DZ60_END        equ   $00003BFF
ROM_DZ32_START      equ   $0000C000     ; Hier beginnt die FLASH beim DZ32
ROM_DZ32_2END       equ   $0000FF7F
ROM_DZ48_START      equ   $00004000     ; Hier beginnt die FLASH beim DZ48
ROM_DZ48_END        equ   $00007FFF




PPAGE_0Start:       equ   $00008000
PPAGE_0End:         equ   $000098FF
PPAGE_0_1Start:     equ   $0000BC00
PPAGE_0_1End:       equ   $0000BFFF
PPAGE_2Start:       equ   $00028000
PPAGE_2End:         equ   $0002BFFF
PPAGE_4Start:       equ   $00048000
PPAGE_4End:         equ   $0004BFFF
PPAGE_5Start:       equ   $00058000
PPAGE_5End:         equ   $0005BFFF
EEPROMStart:        equ   $00003C00
EEPROMEnd:          equ   $00003FFF


VECTOR_START        equ   $FF80