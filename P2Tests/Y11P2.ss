(define (rule20_Test t)
  (cond ((= t 0) 'equal)
        ((< t 0) 'negative)
        (else 'positive)
        )
  )

(rule20_Test 20)

;(newline)
;(display (rule20_Test 20))
;(newline)
;(newline)
