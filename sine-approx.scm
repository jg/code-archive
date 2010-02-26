;approximate sin(x)
;sin(x) ~= x in radians for sufficiently small x
;sin(3 * x/3) = 3*sin(x/3) - 4*sin^3(x/3)

(define (sine angle)
  (if (< (abs angle) 0.01)
      angle
      ((lambda (x) (- (* 3 x) (* 4 (* x x x)))) (sine (/ angle 3.0)))))

; (without the lambda abstraction)
;(define (sine angle)
;  (define (f x) (- (* 3 x) (* 4 (* x x x))))
;  (if (< (abs angle) 0.1)
;      angle
;      (f (sine (/ angle 3.0)))))r
