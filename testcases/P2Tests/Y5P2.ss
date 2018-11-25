(define (main)
  (cdr '(1 2 3))
  (f1)
  )

(define (f1)
  (and f2 f3)
  )

(define (f2)
  (* 5 -5)
  )

(define (f3)
  (round 24.67)
  )

(main)
