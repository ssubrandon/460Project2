(define (sub_five x)
  (if (number? x)
      (if (zero? x)
          -5
          (- x 5)
          )
      'this_function_only_takes_numbers
      )
  )

(define (add_five x)
  (if (number? x)
      (if (zero? x)
          5
          x+5
          )
      'this_function_only_takes_numbers
      )
  )
(define (main)
  (display (sub_five 0))
  (newline)

  (display (sub_five 9))
  (newline)

  (display (sub_five -3))
  (newline)

  (display (add_five 0))
  (newline)
  
  (display (add_five 4))
  (newline)
  
  (display (add_five -3))
  (newline)
  )

(main)
