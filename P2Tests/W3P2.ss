; Group: W 

(define (what_is_this n)
  (cond   ((null? n) 'this_is_null)
          ((number? n) 'this_is_number)
          ((list? n) 'this_is_a_list)
          ((string? n) 'this_is_a_string)
          (else 'dont_know_what_this_is))
  )
(define (main)
  (display (what_is_this "word"))
  (newline)
  
  (display (what_is_this 14))
  (newline)
  
  (display (what_is_this '(a b c)))
  (newline)

  (display (what_is_this '()))
  (newline)
  
  (display (what_is_this 'foo))
  (newline)
  
  (display (what_is_this cdr))
  (newline)
  )

(main)
