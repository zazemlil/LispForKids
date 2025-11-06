(letrec
    (COMPILE (quote 
        (ADD x y) # компилируемая программа (нужно добавить isLiteral в COMP)
    ))
    (COMPILE (
        lambda (E) (
            COMP E (quote ((x y))) (quote Nil) # тестовое окружение ((x y))
        )
    ))
    (COMP (
        lambda (E N C) 
            (cond (ATOM E) 
                (CONS (QUOTE LD) (CONS (LOCATION E N) C))
            (cond (EQUAL (CAR E) (QUOTE QUOTE))
                (CONS (QUOTE LDC) (CONS (CAR (CDR E)) C))
            (cond (EQUAL (CAR E) (QUOTE ADD))
                (COMP (CAR (CDR E)) N (COMP (CAR (CDR (CDR E))) N (CONS (QUOTE ADD) C)))
                (QUOTE -12345))
        ))
    ))
    (LOCATION (LAMBDA (E1 N1)
        (LETREC
            (cond (MEMBER E1 (CAR N1)) 
                (CONS (QUOTE 0) (CONS (POSN E1 (CAR N1)) NIL)) 
                (INCAR (LOCATION E1 (CDR N1)))
            )
            
            (MEMBER (LAMBDA (E N)
                (cond (EQUAL N (QUOTE NIL)) (quote FALSE)
                (cond (EQUAL E (CAR N)) (quote TRUE) (MEMBER E (CDR N))))))
            (POSN (LAMBDA (E N)
                (cond (EQUAL E (CAR N)) (QUOTE 0) (ADD (QUOTE 1) (POSN E (CDR N))))))
            (INCAR (LAMBDA (L) (CONS (ADD (QUOTE 1) (CAR L)) (CDR L))))
        ))
    )
)