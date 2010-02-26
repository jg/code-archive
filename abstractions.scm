;sum abstraction
(define (sum term a next b)
  (if (> a b)
      0
      (+ (term a)
	(sum term (next a) next b))))

(define (sum-i term a next b)
  (define (iter a result)
    (if (> a b)
	result
	(iter (next a) (+ (term a) result))))
  (iter a 0))

;product abstraction
(define (product term a next b)
  (if (> a b)
      1
      (* (term a) 
	(product term (next a) next b))))

(define (product-i term a next b)
  (define (iter a result)
    (if (> a b)
	result
	(iter (next a) (* result (term a)))))
  (iter a 1))
    
;accumulate abstraction (hop)
(define (accumulate combiner null-value term a next b)
  (if (> a b)
      null-value
      (combiner (term a)
	       (accumulate combiner null-value term (next a) next b))))

(define (accumulate-i combiner null-value term a next b)
  (define (iter a result)
    (if (> a b)
        result
        (iter (next a) (combiner (term a)))))
  (iter a null-value))

(define (sum term a next b) (accumulate (lambda (x y) (+ x y)) 0 term a next b))

(define (product term a next b) (accumulate (lambda (x y) (* x y)) 1 term a next b))

(define (filtered-accumulate combiner null-value term a next b predicate)
  (if (> a b)
      null-value
      (if (predicate a)
	  (combiner (term a)
		  (filtered-accumulate combiner null-value term (next a) next b predicate))
	  (filtered-accumulate combiner null-value term (next a) next b predicate))))

(define (even-sum n) (filtered-accumulate (lambda (x y) (+ x y)) 0 (lambda (x) x) 1 (lambda (x) (+ x 1)) n even?))
