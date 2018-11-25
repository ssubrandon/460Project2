(define (isNonZeroNumber n)
  (cond ((number? n)
	(cond ((zero? n) 'This_is_zero)
	      (else 'This_is_a_nonzero_number)
	      ))
  (else 'NaN)
  )  
)

(display (isNonZeroNumber 1))
(newline)

(display (isNonZeroNumber 0))
(newline)

(display (isNonZeroNumber 'wordup))
(newline)

(display (isNonZeroNumber "String"))
(newline)



