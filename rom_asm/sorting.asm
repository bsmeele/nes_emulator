.reserve $00 $0F  // Bubble sort array
.reserve $10 $1F  // Insertion sort array
.reserve $20 $2F  // Quicksort array
.reserve $30 $32  // Array pointer and length

START:
  LDX #0x00
  LDA #0x89
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0xE3
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0xF4
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0x22
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0x02
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0x2C
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0x9B
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0x2F
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0xD8
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0x66
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0xD1
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0x47
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0xBC
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0x3F
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0x5D
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X
  INX
  LDA #0xEA
  STA 0x00,X
  STA 0x010,X
  STA 0x020,X

  LDA #0x00
  STA 0x30
  LDA #0x00
  STA 0x31
  LDA #0x10
  STA 0x32
  JSR BUBBLESORT.START

  LDA #0x10
  STA 0x30
  LDA #0x00
  STA 0x31
  LDA #0x10
  STA 0x32
  JSR INSERTIONSORT

  LDA #0x20
  STA 0x30
  LDA #0x00
  STA 0x31
  LDA #0x10
  STA 0x32
  JSR QUICKSORT

  BRK

.block BUBBLESORT entry=START
START:
  // 0x0030-0x0031 is the pointer to the list
  // n is written to 0x0032 by caller
  LDA #0xFF
  STA 0x33
OUTER_LOOP:
  LDA #0x00
  CMP 0x33
  BEQ END_OUTER
  STA 0x33
  LDY #0x01
INNER_LOOP:
  CPY 0x32
  BCS END_INNER
  DEY
  LDA (0x30),Y
  INY
  CMP (0x30),Y
  BCC END_IF
  BEQ END_IF
IF:
  TAX
  LDA (0x30),Y
  DEY
  STA (0x30),Y
  INY
  TXA
  STA (0x30),Y
  INC 0x33
END_IF:
  INY
  SEC
  BCS INNER_LOOP
END_INNER:
  DEC 0x0032
  SEC
  BCS OUTER_LOOP
END_OUTER:
  RTS

.block INSERTIONSORT
START:
  // 0x0030-0x0031 is the pointer to the list
  // n is written to 0x0032 by caller
  LDY #0x01
OUTER_LOOP:
  CPY 0x32
  BCS END_OUTER
  LDA (0x30),Y
  STA 0x33
  TYA
  TAX
INNER_LOOP:
  CPY #0x00
  BCC END_INNER_1
  BEQ END_INNER_1
  DEY
  LDA (0x30),Y
  CMP 0x33
  BCC END_INNER_0
  BEQ END_INNER_0
  INY
  STA (0x30),Y
  DEY
  SEC
  BCS INNER_LOOP
END_INNER_0:
  INY
END_INNER_1:
  LDA 0x33
  STA (0x30),Y
  TXA
  TAY
  INY
  SEC
  BCS OUTER_LOOP
END_OUTER:
  RTS

.block QUICKSORT entry=SETUP
// 0x0030-0x0031 is the pointer to the list
// The length of the list is written to 0x0032 by the caller
SETUP:
  LDX 0x32
  DEX
  LDY #0x00
START:
  CPY #0x00
  BCC END
  CPX #0x00
  BCC END
  STX tmp_hi
  CPY tmp_hi
  BCS END
  TXA
  PHA
  TYA
  PHA
  JSR PIVOT  // returne value is passed through accumulator
  TAX
  PLA
  TAY
  TXA
  PHA
  JSR START  // returne value is passed through accumulator
  PLA
  TAY
  INY
  PLA
  TAX
  JSR START
END:
  RTS

.block PIVOT entry=START
START:
  // y = lo, x = hi
  LDA (0x30),Y  // Acc = pivot
  DEY  // y = i
  INX  // x = j
LOOP:
I_LOOP:
  INY
  CMP (0x30),Y
  BCC I_LOOP_END
  BEQ I_LOOP_END
  BCS I_LOOP
I_LOOP_END:
  STX tmp_j
  STY tmp_i
  LDX tmp_i  // x = i
  LDY tmp_j  // y = j
J_LOOP:
  DEY
  CMP (0x30),Y
  BCC J_LOOP
J_LOOP_END:
  STY tmp_j
  CPX tmp_j
  BCC SWAP
  TYA
  RTS  // Return with Acc - j
SWAP:
  STA tmp_pivot
  STX tmp_i
  LDA (0x30),Y  // Acc = A[j]
  STA tmp_A_j
  LDY tmp_i
  LDA (0x30),Y  // Acc = A[i]
  LDY tmp_j
  STA (0x30),y
  LDY tmp_i
  LDA tmp_A_j
  STA (0x30),Y
  LDA tmp_pivot
  LDX tmp_j
  CLC
  BCC LOOP
