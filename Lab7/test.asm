
 .CSEG
 .org $000
 jmp reset
 .org $028
 reti

reset:
   ldi r16, 0b10000000
   out ACSR , r16
   ldi r16 , 0x00
   ldi r17 , 0xFF
   out PORTB , r17
   out DDRB , r16
   out PORTA , r17
   out DDRA, r17

   rjmp reset
 
main:
   in   r16, PINB
   out  PORTA, r16        
   jmp main