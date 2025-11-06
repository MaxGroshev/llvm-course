entry
    ALLOCA x1 256 512
    SIM_RAND x2
    SREM x2 x2 246
    SEXT x2 x2
    STi 16763144 256 512 x1 x2 255
    MOV x23 x2 
    MOVi x20 0
    BR BB7
BB7
    MOV x2 x23 
    MOV x3 x20 
    ZEXT x4 x2
    MOVi x14 0
    BR BB11
BB11
    MOV x5 x14
    CMP_EQ x6 x5 256
    BR_COND x6 BB17 BB14
BB14
    CMP_EQ x15 x5 x4 
    TRUNC x16 x5
    MOVi x13 1
    BR BB20
BB17
    AND x7 x3 127
    CMP_EQ x19 x7 0
    BR_COND x19 BB38 BB100
BB100
    MOV x9 x2
    BR BB68
BB20
    MOV x21 x13
    CMP_EQ x22 x21 511
    BR_COND x22 BB23 BB25
BB23
    ADDi x24 x5 1
    ADDi x14 x24 0
    BR BB11
BB25
    LD x27 256 512 x1 x5 x21
    TRUNC x28 x21 
    ADDi x29 x28 -1
    PUT_PIXEL x29 x16 x27
    CMP_EQ x30 x21 447
    SELECT x31 x30 x15 0
    MOVi x12 448
    BR_COND x31 BB32 BB34
BB32
    MOV x33 x12
    MOV x13 x33
    BR BB20
BB34
    ADDi x35 x21 1
    LD x37 256 512 x1 x5 x35
    ST x37 256 512 x1 x5 x21
    MOV x12 x35
    BR BB32
BB38   
    SIM_RAND x39
    SREM x40 x39 246
    SEXT x41 x40
    MOVi x11 0
    BR BB42    
BB42
    MOV x43 x11
    CMP_EQ x44 x43 256
    BR_COND x44 BB55 BB45
BB45
    TRUNC x46 x43
    SUB x47 x40 x46
    SIM_ABS x48 x47
    CMP_SGT x49 x48 35
    BR_COND x49 BB50 BB53
BB50
    SHL x8 x43 11
    ADDi x52 x8 2040
    ST_BT_OFFSET 7522568 x1 x52
    BR BB53
BB53
    ADDi x8 x43 1
    MOV x11 x8
    BR BB42
BB55
    SIM_MIN x7 x2 x40
    SEXT x7 x7
    MOV x10 x7
    BR BB58
BB58
    MOV x59 x10
    SIM_MAX x7 x2 x44
    SEXT x61 x7
    CMP_SLT x8 x59 x61
    BR_COND x8 BB65 BB63
BB63
    STi 16763144 256 512 x1 x41 447
    MOV x9 x40
    BR BB68
BB65
    STi 16763144 256 512 x1 x59 447
    ADDi x7 x59 1
    MOV x10 x7
    BR BB58
BB68
    MOV x8 x9
    SIM_FLUSH
    ADDi x7 x3 1
    MOV x23 x8
    MOV x20 x7
    BR BB7
exit
    EXIT