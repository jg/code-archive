;linear recursion
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

; dx[f(a+dx/2) + f(a+dx+dx/2) + f(a+2dx+dx/2) + ...]
(define (integral f a b dx)
  (define (add-dx x) (+ x dx))
  (* (sum f (+ a (/ dx 2.0)) add-dx b) 
     dx))

;simpson's approximation
; h/3*[y0 + 4y1 + 2y2 + 4y3 + 2y4 + ... + 2y(n-2) + 4y(n-1) + yn]
; where h= (b-a)/n, and y(k) = f(a+kh)
; it converges on the solution quicker than the (integral) routine 
(define (integral-simpson f a b n)
  (define h (/ (- b a) n))
  (* 
   (/ h 3)
   (+
    (* 4 (sum f (+ a h) (lambda (x) (+ x (* 2 h))) (* n h)))
    (* 2 (sum f (+ a (* 2 h)) (lambda (x) (+ x (* 2 h))) (* n h)))
    (f a)
    (f (+ a (* n h))))))
 

